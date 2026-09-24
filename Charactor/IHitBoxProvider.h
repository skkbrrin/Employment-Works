#pragma once
#include <vector>
#include <DirectXCollision.h>
#include <string>

// ƒp[ƒc“–‚½‚è”»’è
struct HitBoxPart
{
    std::string name;
    DirectX::BoundingOrientedBox obb;
    bool isWeapon = false;
    bool isDamageable = false;
    bool hit = false;
};

class IHitBoxProvider
{
public:
    virtual std::vector<HitBoxPart> GetHitBoxes() const = 0;
};


