#include "pch.h"
#include "ResourceManager/Resource.h"
#include "HPUI.h"

void HPUI::Initialize(ID3D11Device* device, ID3D11DeviceContext* context, const wchar_t* texturePath)
{
    DirectX::CreateWICTextureFromFile(
        device,
        texturePath,
        nullptr,
        m_texture.ReleaseAndGetAddressOf()
    );

    m_pos = { 80.0f, 600.0f };
}

void HPUI::Render(DirectX::SpriteBatch* spriteBatch)
{
    for (int i = 0; i < m_hp; ++i)
    {
        spriteBatch->Draw(
            m_texture.Get(),
            m_pos + DirectX::SimpleMath::Vector2(i * 40.0f, 0.0f),
            nullptr,
            DirectX::Colors::White,
            0.0f,
            { 0, 0 },
            1.0f
        );
    }
}
