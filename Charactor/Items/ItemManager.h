#pragma once
#include "Item.h"
#include <vector>

class Player;

using namespace DirectX::SimpleMath;

class ItemManager
{
public:
    void Spawn(Item::Type type, const Matrix& world);

    void Update(float elapsedTime, Player* player);

    void Render(ID3D11DeviceContext* context, DirectX::CommonStates* states, Matrix view, Matrix proj);

    void CheckPickup(Player* player);
    
private:
    std::vector<std::unique_ptr<Item>> items;
};

