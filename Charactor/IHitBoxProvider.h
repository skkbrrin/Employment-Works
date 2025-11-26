#pragma once
#include <vector>
#include <DirectXCollision.h>

struct HitBoxPart
{
    std::string name;
    DirectX::BoundingOrientedBox obb;
    bool isWeapon = false;
    bool isDamageable = false;
};

class IHitBoxProvider
{
public:
    virtual std::vector<HitBoxPart> GetHitBoxes() const = 0;
};
