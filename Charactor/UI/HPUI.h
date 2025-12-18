#pragma once
#include <d3d11_1.h>
#include <SpriteBatch.h>
#include "SimpleMath.h"
#include <wrl/client.h>

class HPUI
{
public:
    void Initialize(
        ID3D11Device* device,
        ID3D11DeviceContext* context,
        const wchar_t* texturePath);

    void SetHP(int hp);
    void Render(DirectX::SpriteBatch* spriteBatch);

private:
    static constexpr int MAX_HP = 3;
    int m_hp = MAX_HP;

    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;
    DirectX::SimpleMath::Vector2 m_pos;
};
