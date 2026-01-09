//-------------------------------------------------------------------------------------
// File: ShadowSprite.h
//
// simple drop-shadow quad renderer (like TextureSprite style)
//-------------------------------------------------------------------------------------

#pragma once
#include <memory>
#include <SimpleMath.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>

class ShadowSprite
{
public:
    ShadowSprite(ID3D11Device* device, ID3D11DeviceContext* context);
    ~ShadowSprite();

    // 四角形のサイズ
    void SetSize(float w, float h);
    // 影のオフセット
    void SetOffset(float x, float y);

    // 四角形描画
    void Render(const DirectX::SimpleMath::Vector2& pos, float groundY, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj);

    void Finalize();

private:
    ID3D11Device* m_device;
    ID3D11DeviceContext* m_context;

    std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> m_batch;
    std::unique_ptr<DirectX::AlphaTestEffect> m_effect;
    std::unique_ptr<DirectX::CommonStates> m_states;

    DirectX::XMFLOAT4 m_color = { 0,0,0,0 };
    float m_width = 2.5f; // 影の横幅
    float m_height = 2.5f; // 影の縦幅
    float m_offsetX = 0.0f; // 初期値
    float m_offsetY = 0.0f; // 初期値

    Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;
};
