#include "pch.h"
#include "TimerUI.h"

void TimerUI::Initialize(ID3D11Device* device)
{
    // テクスチャ読み込み
    DirectX::CreateWICTextureFromFile(
        device,
        L"Resources/Textures/CircleMeter01.png",
        nullptr,
        m_texture.ReleaseAndGetAddressOf()
    );

    // PixelShader 読み込み（事前コンパイル済み .cso 推奨）
    Microsoft::WRL::ComPtr<ID3DBlob> psBlob;
    //D3DReadFileToBlob(L"CircleGaugePS.cso", psBlob.GetAddressOf());

    device->CreatePixelShader(
        psBlob->GetBufferPointer(),
        psBlob->GetBufferSize(),
        nullptr,
        m_pixelShader.ReleaseAndGetAddressOf()
    );

    // ConstantBuffer 作成
    D3D11_BUFFER_DESC desc{};
    desc.ByteWidth = sizeof(TimerCB);
    desc.Usage = D3D11_USAGE_DYNAMIC;
    desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    device->CreateBuffer(
        &desc,
        nullptr,
        m_constantBuffer.ReleaseAndGetAddressOf()
    );
}

void TimerUI::Update(float dt)
{
    if (m_timer <= 0.0f) return;

    m_timer -= dt;
    m_timer = std::max(0.0f, m_timer);
}

void TimerUI::Render(DirectX::SpriteBatch* spriteBatch)
{
    
}

void TimerUI::Start(float time)
{
    m_maxTime = time;
    m_timer = time;
}