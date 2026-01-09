#include "pch.h"
#include "ItemManager.h"
#include "Charactor/Player/Player.h"
#include "WoodManager.h"

void ItemManager::Initialize(ID3D11Device* device, ID3D11DeviceContext* context)
{
    m_device = device;
    m_context = context;
}

void ItemManager::Update(float elapsedTime, Player* player)
{
    for (auto& item : items)
        item->Update(elapsedTime);

    CheckPickup(player);

    // 拾われたアイテム削除
    items.erase(
        std::remove_if(items.begin(), items.end(),
            [](const std::unique_ptr<Item>& i) { return i->IsPickedUp(); }),
        items.end()
    );
}

void ItemManager::Render(ID3D11DeviceContext* context, DirectX::CommonStates* states, Matrix view, Matrix proj)
{
    for (auto& item : items)
        item->Render(context, states, view, proj);
}

void ItemManager::CheckPickup(Player* player)
{
   // プレイヤーの本当のワールド位置を取得
    auto* body = player->GetPart("Body");
    Vector3 playerRealPos = body
        ? body->GetWorldMatrix().Translation()
        : player->GetWorldMatrix().Translation(); // フォールバック

    for (auto& item : items)
    {
        if (item->IsPickedUp()) continue;

        float dist = (item->GetWorldMatrix().Translation()
            - playerRealPos).Length();

        if (dist < 2.0f)
        {
            if (item->GetType() == Item::Type::Wood)
                WoodManager::Instance().Add(1);

            item->SetPickedUp(true);
        }
    }
}

void ItemManager::SpawnWithVelocity(Item::Type type, const Vector3& pos, const Vector3& vel)
{
    auto item = std::make_unique<Item>(type, pos, vel);
    item->Initialize(m_device, m_context);
    items.emplace_back(std::move(item));
}

void ItemManager::CollectAll(Player* player)
{
    // プレイヤーに加算
    WoodManager::Instance().Add(items.size());

    // 全削除
    items.clear();
}
