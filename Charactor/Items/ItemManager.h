#pragma once
#include "Item.h"
#include <vector>

class Player;

using namespace DirectX::SimpleMath;

class ItemManager
{
public:
    void Initialize(ID3D11Device* device, ID3D11DeviceContext* context);

    void Update(float elapsedTime, Player* player);

    void Render(ID3D11DeviceContext* context, DirectX::CommonStates* states, Matrix view, Matrix proj);

    void CheckPickup(Player* player);

    void SpawnWithVelocity(Item::Type type, const Vector3& pos, const Vector3& vel);
    
    void CollectAll(Player* player);
private:
    std::vector<std::unique_ptr<Item>> items;
    ID3D11Device* m_device = nullptr;
    ID3D11DeviceContext* m_context = nullptr;
};

