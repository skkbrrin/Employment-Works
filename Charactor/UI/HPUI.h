#pragma once
#include <d3d11_1.h>
#include <SpriteBatch.h>
#include "SimpleMath.h"
#include <wrl/client.h>

template <typename T>
T Clamp(T value, T minValue, T maxValue)
{
    if (value < minValue) return minValue;
    if (value > maxValue) return maxValue;
    return value;
}

class HPUI
{
public:
    void Initialize(
        ID3D11Device* device,
        ID3D11DeviceContext* context,
        const wchar_t* texturePath);

    void Render(DirectX::SpriteBatch* spriteBatch, int currentHP, int maxHP);

private:
    static constexpr int MAX_HP = 3;
    int m_hp = MAX_HP;

    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;
    DirectX::SimpleMath::Vector2 m_pos;
};
