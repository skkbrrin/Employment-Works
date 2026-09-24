#include "pch.h"
#include "ItemManager.h"
#include "Charactor/Player/Player.h"
#include "WoodManager.h"


// 初期化
void ItemManager::Initialize(ID3D11Device* device, ID3D11DeviceContext* context)
{
    m_device = device;
    m_context = context;
}

// 更新
void ItemManager::Update(float elapsedTime, Player* player)
{
    // 更新
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

// 描画
void ItemManager::Render(ID3D11DeviceContext* context, DirectX::CommonStates* states, Matrix view, Matrix proj)
{
    for (auto& item : items)
        item->Render(context, states, view, proj);
}

// 拾う
void ItemManager::CheckPickup(Player* player)
{
   // プレイヤーの本当のワールド位置を取得
    auto* body = player->GetPart("Body");
    Vector3 playerRealPos = body
        ? body->GetWorldMatrix().Translation()
        : player->GetWorldMatrix().Translation(); // フォールバック

    // プレイヤーの範囲にあれば拾える
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

// アイテムを出現させる
void ItemManager::SpawnWithVelocity(Item::Type type, const Vector3& pos, const Vector3& vel)
{
    auto item = std::make_unique<Item>(type, pos, vel);
    item->Initialize(m_device, m_context);
    items.emplace_back(std::move(item));
}

// デバッグ用の全て集める関数
void ItemManager::CollectAll(Player* player)
{
    // プレイヤーに加算
    WoodManager::Instance().Add(items.size());

    // 全削除
    items.clear();
}
