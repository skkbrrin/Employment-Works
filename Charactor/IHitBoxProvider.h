#pragma once
#include <vector>
#include <DirectXCollision.h>

class IHitBoxProvider
{
public:
    virtual ~IHitBoxProvider() {}

    virtual std::vector<DirectX::BoundingOrientedBox>
        GetLocalHitBoxes() const = 0;

    virtual std::vector<DirectX::BoundingOrientedBox>
        GetWorldHitBoxes() const = 0;
};
