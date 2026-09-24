#pragma once
#include "Item.h"
#include <vector>

class Player;

using namespace DirectX::SimpleMath;

class ItemManager
{
public:
    // 初期化
    void Initialize(ID3D11Device* device, ID3D11DeviceContext* context);

    // 更新
    void Update(float elapsedTime, Player* player);

    // 描画
    void Render(ID3D11DeviceContext* context, DirectX::CommonStates* states, Matrix view, Matrix proj);

    // 拾う
    void CheckPickup(Player* player);

    // アイテムを出現させる
    void SpawnWithVelocity(Item::Type type, const Vector3& pos, const Vector3& vel);
    
    // 全てのアイテムを集める
    void CollectAll(Player* player);
private:
    std::vector<std::unique_ptr<Item>> items;
    ID3D11Device* m_device = nullptr;
    ID3D11DeviceContext* m_context = nullptr;
};

