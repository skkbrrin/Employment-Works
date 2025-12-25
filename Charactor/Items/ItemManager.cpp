#include "pch.h"
#include "ItemManager.h"
#include "Charactor/Player/Player.h"

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

        if (dist < 0.5f)
        {
            if (item->GetType() == Item::Type::Wood)
                player->AddWood(1);

            item->SetPickedUp(true);
        }
    }
}

void ItemManager::SpawnWithVelocity(Item::Type type, const Vector3& pos, const Vector3& vel)
{
    items.emplace_back(std::make_unique<Item>(type, pos, vel));
}