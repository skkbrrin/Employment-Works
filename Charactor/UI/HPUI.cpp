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

    m_pos = { 23.5f, 25.0f };
}

void HPUI::Render( DirectX::SpriteBatch* spriteBatch, int currentHP, int maxHP)
{
    float scale = 2.0f;
    float radius = 50.0f;

    // HP0‚È‚ç‰f‚³‚È‚¢
    if (maxHP <= 0) return;

    // HP‚ðŽO•ªŠ„‚µ‚Ä•\Ž¦
    int hpStage = (currentHP * 3) / maxHP;
    hpStage = Clamp(hpStage, 0, 3);


    for (int i = 0; i < hpStage; ++i)
    {
        float angle = (2.0f * DirectX::XM_PI / 3) * i;

        DirectX::SimpleMath::Vector2 offset(
            radius,
            radius
        );
       
        spriteBatch->Draw(
            m_texture.Get(),
            m_pos + offset,
            nullptr,
            DirectX::Colors::White,
            angle,
            { 60.0f / 2.0f, 60.0f / 2.0f },
            scale
        );
    }
}
