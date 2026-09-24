#include "pch.h"
#include "Field.h"
#include "ResourceManager/Resource.h"

int Field::ENEMY_COUNT = 35; //< 敵の数
int Field::TREE_COUNT = 45; //< 木の数

// 当たり判定ボックス
inline DirectX::SimpleMath::Matrix MakeBoxMatrix(const DirectX::BoundingBox& box)
{
    using namespace DirectX::SimpleMath;
    return Matrix::CreateScale(box.Extents * 2.0f) * Matrix::CreateTranslation(box.Center.x, box.Center.y + 0.7f, box.Center.z - 0.3f);
}

// 初期化
void Field::Initialize(ID3D11Device* device, ID3D11DeviceContext* context, DX::DeviceResources* dr)
{
    // プレイヤーの初期状態
    if(m_player == nullptr)
    {
        m_player = new Player;
    }
    m_player->Initialize(device, dr);

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

        e->Initialize(device);

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

        t->Initialize(device);

        m_tries.push_back(std::move(t));
    }

    // アイテムのロード
    Item::LoadModels(device);

    m_itemManager.Initialize(device, context);

    // 目標数の表示
    WoodManager::Instance().SetClearCount(30);

    // 出口の湖の初期化
    std::unique_ptr<DirectX::EffectFactory> fx = std::make_unique<DirectX::EffectFactory>(device);
    fx->SetDirectory(L"Resources/Models");
    m_lake = DirectX::Model::CreateFromSDKMESH(device, L"Resources/Models/Lake.sdkmesh", *fx);

    // コライダー
    m_debugOBB = DirectX::GeometricPrimitive::CreateBox(context, DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));

    m_debugSphere =
        DirectX::GeometricPrimitive::CreateSphere(
            context,
            1.0f,    // 半径1の球（描画時にスケール）
            16       // 分割数（軽めでOK）
        );
}

// 更新
void Field::Update(float elapsedTime)
{
    auto kb = DirectX::Keyboard::Get().GetState();

    // デバッグ用
    // オブジェクトの全消去
    if (kb.F1)
    {
        for (auto& enemy : m_enemies)
        {
            enemy->TakeDamage(999999);
        }

        for (auto& tree : m_tries)
        {
            tree->TakeDamage(999999);
        }

    }
    // アイテムの全回収
    if (kb.F9)
    {
        m_itemManager.CollectAll(m_player);
    }

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

    // もし材木が一定数あつまったら
    if (!m_lakeAppeared && WoodManager::Instance().IsComplete())
    {
        // 出現許可
        m_lakeAppeared = true;
    }

    // 湖に入ったらクリア
    if (m_lakeAppeared)
    {
        DirectX::SimpleMath::Vector3 playerPos = m_player->GetPosition();
        DirectX::SimpleMath::Vector3 lakePos = DirectX::SimpleMath::Vector3::Zero;

        float distance =
            (playerPos - lakePos).Length();

        if (distance < 5.0f)
        {
            m_canChangeScene = true;
        }
    }

    // アイテムドロップ
    /*for (auto& enemy : m_enemies)
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
    }*/
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

// 描画
void Field::Render(ID3D11DeviceContext* context, DirectX::CommonStates* states, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
    // プレイヤー描画
    m_player->RenderP(context, states, view, proj);

    // 敵の描画
    for (auto& e : m_enemies)
        e->RenderE(context, states, view, proj);

    // 木の描画
    for (auto& t : m_tries)
        t->RenderT(context, states, view, proj);

    // アイテムの描画
    m_itemManager.Render(context, states, view, proj);

    // 湖の描画
    if (m_lakeAppeared)
    {
        DirectX::SimpleMath::Matrix world_L =
            DirectX::SimpleMath::Matrix::CreateScale(10.0f) *
            DirectX::SimpleMath::Matrix::CreateTranslation( DirectX::SimpleMath::Vector3( 0.0f, -0.5f, 0.0f ));

        m_lake->Draw(context, *states, world_L, view, proj);
    }

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

    // プレイヤー当たり判定の描画（攻撃の際に見えるようにするため）
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

//#if defined(_DEBUG)
//    if (m_player->GetDashAttacking())
//    {
//        using namespace DirectX::SimpleMath;
//
//        float r = m_player->GetPowerAttackRadius();
//
//        Matrix world =
//            Matrix::CreateScale(r) *
//            Matrix::CreateTranslation(m_player->GetPosition());
//
//        m_debugSphere->Draw(
//            world,
//            view,
//            proj,
//            DirectX::XMVECTORF32{ 1, 1, 1, 0.5f }
//        );
//    }
//#endif
}

//　当たったかどうかの判定
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
                        m_player->TakeDamage(1.0f, enemy->GetPosition());
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

    //if (m_player->GetDashAttacking())
    //{
    //    const float radius = m_player->GetPowerAttackRadius();
    //    Vector3 center = m_player->GetPosition();

    //    for (auto& enemy : m_enemies)
    //    {
    //        Vector3 toEnemy = enemy->GetPosition() - center;
    //        float distSq = toEnemy.LengthSquared();

    //        if (distSq <= radius * radius)
    //        {
    //            // ダメージ
    //            enemy->TakeDamage(20);

    //            // ノックバック
    //            toEnemy.Normalize();
    //        }
    //    }

    //    // 木も同様
    //    for (auto& tree : m_tries)
    //    {
    //        Vector3 toTree = tree->GetPosition() - center;
    //        float distSq = toTree.LengthSquared();

    //        if (distSq <= radius * radius)
    //        {
    //            // ダメージ
    //            tree->TakeDamage(20);
    //        }
    //    }
    //}
}


// 当たり判定の更新
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
    float vy = (rand() % 30) * 0.05f + 15.0f;

    Vector3 vel(vx, vy, vz);

    m_itemManager.SpawnWithVelocity(Item::Type::Wood, pos, vel);
}
