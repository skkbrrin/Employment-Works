#include "pch.h"
#include "Field.h"

inline DirectX::SimpleMath::Matrix MakeBoxMatrix(const DirectX::BoundingBox& box)
{
    using namespace DirectX::SimpleMath;
    return Matrix::CreateScale(box.Extents * 2.0f) * Matrix::CreateTranslation(box.Center.x, box.Center.y + 0.7f, box.Center.z - 0.3f);
}

void Field::Initialize(ID3D11Device* device, ID3D11DeviceContext* context)
{
    m_player.Initialize(device);

    for (int i = 0; i < 10; i++)
    {
        auto e = std::make_unique<Enemy>();
        e->Initialize(device);

        float x = (rand() % 500 - 250) * 0.3f;
        float z = (rand() % 500 - 250) * 0.3f;
        e->SetPosition({ x, 0.0f, z });

        float rot = (rand() % 360);
        e->RotateY(rot);

        m_enemies.push_back(std::move(e));
    }

    Item::LoadModels(device);

    m_debugOBB = DirectX::GeometricPrimitive::CreateBox(context, DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));
}

void Field::Update(float elapsedTime)
{
    m_player.Update(elapsedTime);

    for (auto& e : m_enemies)
        e->Update(elapsedTime);

    CheckCollision();

    // アイテムドロップ
    for (auto& enemy : m_enemies)
    {
        if (enemy->GetDeleteFlag())
        {
            if (enemy->ShouldDropItem())
            {
                SpawnItem(enemy->GetWorldMatrix());
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
}

void Field::Render(ID3D11DeviceContext* context, DirectX::CommonStates* states, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
    m_player.RenderP(context, states, view, proj);

    for (auto& e : m_enemies)
        e->RenderE(context, states, view, proj);

    for (auto& item : m_items)
        item->Render(context, states, view, proj);

#if defined(_DEBUG)
    auto playerBoxes = m_player.GetHitBoxes();

    for (auto& e : m_enemies)
    {
        auto enemyBoxes = e->GetHitBoxes();

        for (auto& pBox : playerBoxes)
        {
            for (auto& eBox : enemyBoxes)
            {
                bool hit = pBox.obb.Intersects(eBox.obb);

                if (hit)
                {
                    if (pBox.isWeapon && eBox.isDamageable)
                    {
                        e->TakeDamege(10);
                    }
                    else if (eBox.isWeapon && pBox.isDamageable)
                        m_player.TakeDamage(5);
                }

                auto color = hit ? DirectX::Colors::Red : (pBox.isWeapon ? DirectX::Colors::Blue : DirectX::Colors::Green);
                DrawHitBox(context, states, pBox.obb, view, proj, color);

                color = hit ? DirectX::Colors::Red : (eBox.isWeapon ? DirectX::Colors::Blue : DirectX::Colors::Green);
                DrawHitBox(context, states, eBox.obb, view, proj, color);
            }
        }
    }

#endif
}


void Field::CheckCollision()
{
    // プレイヤーのヒットボックス取得
    auto playerBoxes = m_player.GetHitBoxes();

    for (auto& enemy : m_enemies)
    {
        auto enemyBoxes = enemy->GetHitBoxes();

        // プレイヤーの各パーツと敵の各パーツをチェック
        for (auto& pBox : playerBoxes)
        {
            for (auto& eBox : enemyBoxes)
            {
                if (pBox.obb.Intersects(eBox.obb))
                {
                    // 斧が敵に当たった
                    if (pBox.isWeapon && eBox.isDamageable)
                        enemy->TakeDamage(10);

                    // 敵の手がプレイヤーに当たった
                    if (pBox.isDamageable && eBox.isWeapon)
                        m_player.TakeDamage(10);
                }
            }
        }
    }
}

// アイテムの出現
void Field::SpawnItem(const DirectX::SimpleMath::Matrix& world)
{
    // 種類を決める
    Item::Type type = Item::Type::Wood;

    m_items.push_back(
        std::make_unique<Item>(type, world)
    );
}
