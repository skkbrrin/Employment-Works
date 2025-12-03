#include "pch.h"
#include "ItemManager.h"
#include "Charactor/Player/Player.h"

void ItemManager::Spawn(Item::Type type, const Matrix& world)
{
    items.emplace_back(std::make_unique<Item>(type, world));
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
    for (auto& item : items)
    {
        if (item->IsPickedUp()) continue;

        float dist = (item->GetWorldMatrix().Translation()
            - player->GetWorldMatrix().Translation()).Length();

        if (dist < 2.0f) // 拾える距離
        {
            // 薪ならカウントする
            if (item->GetType() == Item::Type::Wood)
            {
                player->AddWood(1);
            }

            item->SetPickedUp(true);  // 演出的には拾った扱い
        }
    }
}