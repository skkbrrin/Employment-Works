//-------------------------------------------------------------------------------------
// File: TextureAlpha.h
//
// render floor by primitive
//
//-------------------------------------------------------------------------------------

#include "pch.h"
#include "TextureAlpha.h"
#include <PrimitiveBatch.h> 
#include <VertexTypes.h> 
#include <WICTextureLoader.h> 

using namespace DirectX;

/// <summary>
/// Constractor
/// </summary>
TextureAlpha::TextureAlpha(ID3D11Device1* device, ID3D11DeviceContext* context)
	: m_context(context)
	, m_device(device)
	, m_timer(0)
	, m_color()
{
	m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(context);
}

/// <summary>
/// Destractor
/// </summary>
TextureAlpha::~TextureAlpha()
{
	Finalize();
}

bool TextureAlpha::Load(const wchar_t* texture)
{
	HRESULT hr = CreateWICTextureFromFile(
		m_device,
		texture,
		nullptr,
		m_SRV.ReleaseAndGetAddressOf());

	return SUCCEEDED(hr); // 成功ならtrueを返す関数
}

void TextureAlpha::Update(float elapsedTime)
{
	m_timer += elapsedTime;
	// アルファ値を 0～1 の範囲で変化させる（点滅）
	float alpha = abs(sinf(m_timer)); // speedで速さ調整
	m_color = { alpha, alpha, alpha, alpha };
}


/// <summary>
/// Render 
/// </summary>
void TextureAlpha::Render(DirectX::SimpleMath::Vector2 pos, float scale)
{
	m_spriteBatch->Begin();

	m_spriteBatch->Draw(m_SRV.Get(),
		pos, nullptr, m_color, 0.0f, SimpleMath::Vector2(0, 0),
		scale);

	m_spriteBatch->End();
}



/// <summary>
/// Finalize 
/// </summary>
void TextureAlpha::Finalize()
{
	m_SRV.Reset();
	m_spriteBatch.reset();
}
