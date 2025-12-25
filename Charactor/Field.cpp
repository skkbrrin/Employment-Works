#include "pch.h"
#include "Field.h"

int Field::ENEMY_COUNT = 35;
int Field::TREE_COUNT = 45;

inline DirectX::SimpleMath::Matrix MakeBoxMatrix(const DirectX::BoundingBox& box)
{
    using namespace DirectX::SimpleMath;
    return Matrix::CreateScale(box.Extents * 2.0f) * Matrix::CreateTranslation(box.Center.x, box.Center.y + 0.7f, box.Center.z - 0.3f);
}

void Field::Initialize(ID3D11Device* device, ID3D11DeviceContext* context)
{
    // プレイヤーの初期状態
    if(m_player == nullptr)
    {
        m_player = new Player;
    }
    m_player->Initialize(device);

    // 敵の生成数、初期化
    for (int i = 0; i < ENEMY_COUNT; i++)
    {
        auto e = std::make_unique<Enemy>();
        e->Initialize(device);
        e->SetPlayer(m_player);

        float x = (rand() % 500 - 250) * 0.3f;
        float z = (rand() % 500 - 200) * 0.3f;
        e->SetPosition({ x, 0.0f, z });

        float rot = (rand() % 360);
        e->RotateY(rot);

        m_enemies.push_back(std::move(e));
    }

    // 木の生成数、初期化
    for (int i = 0; i < TREE_COUNT; i++)
    {
        auto t = std::make_unique<Tree>();
        t->Initialize(device);
        t->SetPlayer(m_player);

        float x = (rand() % 500 - 250) * 0.3f;
        float z = (rand() % 500 - 250) * 0.3f;
        t->SetPosition({ x, 0.0f, z });

        float rot = (rand() % 360);
        t->SetRotation(Quaternion::CreateFromAxisAngle(Vector3::UnitY, rot));

        m_tries.push_back(std::move(t));
    }

    // アイテムのロード
    Item::LoadModels(device);

    // コライダー
    m_debugOBB = DirectX::GeometricPrimitive::CreateBox(context, DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));

    m_debugSphere =
        DirectX::GeometricPrimitive::CreateSphere(
            context,
            1.0f,    // 半径1の球（描画時にスケール）
            16       // 分割数（軽めでOK）
        );
}

void Field::Update(float elapsedTime)
{
    // プレイヤーの更新
    m_player->Update(elapsedTime);

    // 敵の更新
    for (auto& e : m_enemies)
    {
        e->Update(elapsedTime);
    }

    for (auto& t : m_tries)
        t->Update(elapsedTime);

    // 当たり判定
    CheckCollision();

    m_itemManager.Update(elapsedTime, m_player);

    DirectX::SimpleMath::Matrix ItemWorld = DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3(0, 1, 0));

    // アイテムドロップ
    for (auto& enemy : m_enemies)
    {
        if (enemy->GetDeleteFlag() && enemy->ShouldDropItem())
        {
            int dropCount = 12 + (rand() % 3);

            for (int i = 0; i < dropCount; i++)
            {
                Vector3 pos = enemy->GetPosition();
                Vector3 vel(
                    ((rand() % 100) - 50) * 0.05f,
                    (rand() % 30) * 0.1f + 2.0f,
                    ((rand() % 100) - 50) * 0.05f
                );

                m_itemManager.SpawnWithVelocity(Item::Type::Wood, pos, vel);
            }
        }
    }
    for (auto& tree : m_tries)
    {
        if (tree->GetDeleteFlag() && tree->ShouldDropItem())
        {
            int dropCount = (rand() % 3);

            for (int i = 0; i < dropCount; i++)
            {
                Vector3 pos = tree->GetPosition();
                Vector3 vel(
                    ((rand() % 100) - 50) * 0.05f,
                    (rand() % 30) * 0.1f + 2.0f,
                    ((rand() % 100) - 50) * 0.05f
                );

                m_itemManager.SpawnWithVelocity(Item::Type::Wood, pos, vel);
            }
        }
    }

    // 死亡した敵の消去
    m_enemies.erase(
        std::remove_if(m_enemies.begin(), m_enemies.end(),
            [](const std::unique_ptr<Enemy>& e)
            {
                return e->GetDeleteFlag();
            }),
        m_enemies.end()
    );

    // 切った木の消去
    m_tries.erase(
        std::remove_if(m_tries.begin(), m_tries.end(),
            [](const std::unique_ptr<Tree>& t)
            {
                return t->GetDeleteFlag();
            }),
        m_tries.end()
    );
}

void Field::Render(ID3D11DeviceContext* context, DirectX::CommonStates* states, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
    m_player->RenderP(context, states, view, proj);

    for (auto& e : m_enemies)
        e->RenderE(context, states, view, proj);

    for (auto& t : m_tries)
        t->RenderT(context, states, view, proj);

    m_itemManager.Render(context, states, view, proj);

    //当たり判定ボックスの描画
#if defined(_DEBUG)
    auto playerBoxes = m_player->GetHitBoxes();

    for (auto& enemy : m_enemies)
    {
        auto enemyBoxes = enemy->GetHitBoxes();

        for (auto& pBox : playerBoxes)
        {
            auto color = pBox.hit ? DirectX::Colors::Red
                : (pBox.isWeapon ? DirectX::Colors::Blue : DirectX::Colors::Green);

            DrawHitBox(context, states, pBox.obb, view, proj, color);
        }

        for (auto& eBox : enemyBoxes)
        {
            auto color = eBox.hit ? DirectX::Colors::Red
                : (eBox.isWeapon ? DirectX::Colors::Blue : DirectX::Colors::Green);

            DrawHitBox(context, states, eBox.obb, view, proj, color);
        }
    }

    for (auto& tree : m_tries)
    {
        auto treeBoxes = tree->GetHitBoxes();

        for (auto& pBox : playerBoxes)
        {
            auto color = pBox.hit ? DirectX::Colors::Red
                : (pBox.isWeapon ? DirectX::Colors::Blue : DirectX::Colors::Green);

            DrawHitBox(context, states, pBox.obb, view, proj, color);
        }

        for (auto& tBox : treeBoxes)
        {
            auto color = tBox.hit ? DirectX::Colors::Red
                : (tBox.isWeapon ? DirectX::Colors::Blue : DirectX::Colors::Green);

            DrawHitBox(context, states, tBox.obb, view, proj, color);
        }
    }
#endif

#if defined(_DEBUG)
    if (m_player->GetPowerAttacking())
    {
        using namespace DirectX::SimpleMath;

        float r = m_player->GetPowerAttackRadius();

        Matrix world =
            Matrix::CreateScale(r) *
            Matrix::CreateTranslation(m_player->GetPosition());

        m_debugSphere->Draw(
            world,
            view,
            proj,
            DirectX::XMVECTORF32{1, 1, 1, 0.5f}
        );
    }

    if (m_player->GetDashAttacking())
    {
        using namespace DirectX::SimpleMath;

        float r = m_player->GetPowerAttackRadius();

        Matrix world =
            Matrix::CreateScale(r) *
            Matrix::CreateTranslation(m_player->GetPosition());

        m_debugSphere->Draw(
            world,
            view,
            proj,
            DirectX::XMVECTORF32{ 1, 1, 1, 0.5f }
        );
    }
#endif
}


void Field::CheckCollision()
{
    auto playerBoxes = m_player->GetHitBoxes();

    for (auto& enemy : m_enemies)
    {
        auto enemyBoxes = enemy->GetHitBoxes();

        for (auto& pBox : playerBoxes)
        {
            for (auto& eBox : enemyBoxes)
            {
                bool hit = pBox.obb.Intersects(eBox.obb);

                pBox.hit = hit;
                eBox.hit = hit;

                if (hit)
                {
                    if (pBox.isWeapon && eBox.isDamageable)
                    {
                        enemy->TakeDamage(10);
                    }

                    if (pBox.isDamageable && eBox.isWeapon)
                        m_player->TakeDamage(0.1f, enemy->GetPosition());
                }
            }
        }
    }

    for (auto& tree : m_tries)
    {
        auto treeBoxes = tree->GetHitBoxes();

        for (auto& pBox : playerBoxes)
        {
            for (auto& tBox : treeBoxes)
            {
                bool hit = pBox.obb.Intersects(tBox.obb);

                pBox.hit = hit;
                tBox.hit = hit;

                if (hit)
                {
                    if (pBox.isWeapon && tBox.isDamageable)
                    {
                        tree->TakeDamage(10);
                    }
                }
            }
        }
    }

    if (m_player->GetPowerAttacking())
    {
        const float radius = m_player->GetPowerAttackRadius();
        Vector3 center = m_player->GetPosition();

        for (auto& enemy : m_enemies)
        {
            Vector3 toEnemy = enemy->GetPosition() - center;
            float distSq = toEnemy.LengthSquared();

            if (distSq <= radius * radius)
            {
                // ダメージ
                enemy->TakeDamage(20);

                // ノックバック
                toEnemy.Normalize();
            }
        }

        // 木も同様
        for (auto& tree : m_tries)
        {
            Vector3 toTree = tree->GetPosition() - center;
            float distSq = toTree.LengthSquared();

            if (distSq <= radius * radius)
            {
                // ダメージ
                tree->TakeDamage(20);
            }
        }
    }

    if (m_player->GetDashAttacking())
    {
        const float radius = m_player->GetPowerAttackRadius();
        Vector3 center = m_player->GetPosition();

        for (auto& enemy : m_enemies)
        {
            Vector3 toEnemy = enemy->GetPosition() - center;
            float distSq = toEnemy.LengthSquared();

            if (distSq <= radius * radius)
            {
                // ダメージ
                enemy->TakeDamage(20);

                // ノックバック
                toEnemy.Normalize();
            }
        }

        // 木も同様
        for (auto& tree : m_tries)
        {
            Vector3 toTree = tree->GetPosition() - center;
            float distSq = toTree.LengthSquared();

            if (distSq <= radius * radius)
            {
                // ダメージ
                tree->TakeDamage(20);
            }
        }
    }
}


void Field::UpdateHitBox()
{
    auto playerBoxes = m_player->GetHitBoxes();

    for (auto& enemy : m_enemies)
    {
        auto enemyBoxes = enemy->GetHitBoxes();

        for (auto& pBox : playerBoxes)
        {
            for (auto& eBox : enemyBoxes)
            {
                bool hit = pBox.obb.Intersects(eBox.obb);

                if (hit)
                {
                    if (pBox.isWeapon && eBox.isDamageable)
                    {
                        enemy->TakeDamage(10);
                    }
                    else if (eBox.isWeapon && pBox.isDamageable)
                    {
                        m_player->TakeDamage(5, enemy->GetPosition());
                    }
                }

                pBox.hit = hit;
                eBox.hit = hit;
            }
        }
    }

    for (auto& tree : m_tries)
    {
        auto treeBoxes = tree->GetHitBoxes();

        for (auto& pBox : playerBoxes)
        {
            for (auto& tBox : treeBoxes)
            {
                bool hit = pBox.obb.Intersects(tBox.obb);

                if (hit)
                {
                    if (pBox.isWeapon && tBox.isDamageable)
                    {
                        tree->TakeDamage(10);
                    }
                }

                pBox.hit = hit;
                tBox.hit = hit;
            }
        }
    }
}

// アイテムの出現
void Field::SpawnItem(const DirectX::SimpleMath::Matrix& world)
{
    using namespace DirectX::SimpleMath;

    // ワールド行列から位置を抽出
    Vector3 pos = world.Translation();

    // 水平方向ランダム
    float vx = ((rand() % 100) - 50) * 0.03f;  // ±1.5くらい
    float vz = ((rand() % 100) - 50) * 0.03f;

    // 上方向に少し跳ねる
    float vy = (rand() % 30) * 0.05f + 5.0f;

    Vector3 vel(vx, vy, vz);

    m_itemManager.SpawnWithVelocity(Item::Type::Wood, pos, vel);
}
