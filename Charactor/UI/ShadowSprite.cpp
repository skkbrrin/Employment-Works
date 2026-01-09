//-------------------------------------------------------------------------------------
// File: ShadowSprite.cpp
//-------------------------------------------------------------------------------------

#include "pch.h"
#include "ShadowSprite.h"
#include "ResourceManager/Resource.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

ShadowSprite::ShadowSprite(ID3D11Device* device, ID3D11DeviceContext* context)
    : m_device(device)
    , m_context(context)
{
    // アルファエフェクト
    m_effect = std::make_unique<AlphaTestEffect>(device);
    // アルファが0より大きければ描画
    m_effect->SetAlphaFunction(D3D11_COMPARISON_GREATER);
    m_effect->SetReferenceAlpha(10);

    //InputLayout
    void const* shaderByCode;
    size_t byteCodeLength;
    m_effect->GetVertexShaderBytecode(&shaderByCode, &byteCodeLength);

    device->CreateInputLayout(
        VertexPositionTexture::InputElements,
        VertexPositionTexture::InputElementCount,
        shaderByCode,
        byteCodeLength,
        m_inputLayout.GetAddressOf()
    );

    // 共通ステート
    m_states = std::make_unique<CommonStates>(device);

    // テクスチャ
    //CreateWICTextureFromFile(device, L"Resources/Textures/shadow.png", nullptr, m_texture.ReleaseAndGetAddressOf());
    m_effect->SetTexture(m_texture.Get());

    // プリミティブバッチ
    m_batch = std::make_unique<PrimitiveBatch<VertexPositionTexture>>(context);
}

ShadowSprite::~ShadowSprite()
{
    Finalize();
}

void ShadowSprite::SetSize(float w, float h)
{
    m_width = w;
    m_height = h;
}

void ShadowSprite::SetOffset(float x, float y)
{
    m_offsetX = x;
    m_offsetY = y;
}

void ShadowSprite::Render(const DirectX::SimpleMath::Vector2& pos, float groundY, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj)
{
    float shadowY = groundY + 0.01f;

    float left = pos.x - m_width * 0.5f + m_offsetX;
    float right = pos.x + m_width * 0.5f + m_offsetX;
    float front = pos.y - m_height * 0.5f + m_offsetY;
    float back = pos.y + m_height * 0.5f + m_offsetY;

    VertexPositionTexture v[4] =
    {
        { Vector3(left,  shadowY, front), Vector2(0, 0) },
        { Vector3(right, shadowY, front), Vector2(1, 0) },
        { Vector3(right, shadowY, back),  Vector2(1, 1) },
        { Vector3(left,  shadowY, back),  Vector2(0, 1) },
    };

    // サンプラ
    ID3D11SamplerState* samplers[] = { m_states->LinearClamp() };
    m_context->PSSetSamplers(0, 1, samplers);

    // 深度は参照のみ（床の上に描く）
    m_context->OMSetDepthStencilState(m_states->DepthRead(), 0);

    // 両面描画
    m_context->RSSetState(m_states->CullNone());

    // Effect設定
    m_effect->SetWorld(Matrix::Identity);
    m_effect->SetView(view);
    m_effect->SetProjection(proj);
    m_effect->Apply(m_context);

    m_context->IASetInputLayout(m_inputLayout.Get());

    // 描画
    m_batch->Begin();
    m_batch->DrawQuad(v[0], v[1], v[2], v[3]);
    m_batch->End();
}

void ShadowSprite::Finalize()
{
    m_batch.reset();
}
