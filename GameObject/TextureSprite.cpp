//-------------------------------------------------------------------------------------
// File: TextureSprite.h
//
// render floor by primitive
//
//-------------------------------------------------------------------------------------

#include "pch.h"
#include "TextureSprite.h"
#include <PrimitiveBatch.h> 
#include <VertexTypes.h> 
#include <WICTextureLoader.h> 

using namespace DirectX;

/// <summary>
/// Constractor
/// </summary>
TextureSprite::TextureSprite(ID3D11Device1* device, ID3D11DeviceContext* context)
	: m_context(context)
	, m_device(device)
{
	m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(context);
}

/// <summary>
/// Destractor
/// </summary>
TextureSprite::~TextureSprite()
{
	Finalize();
}

bool TextureSprite::Load(const wchar_t* texture)
{
	HRESULT hr = CreateDDSTextureFromFile(
		m_device,
		texture,
		nullptr,
		m_SRV.ReleaseAndGetAddressOf());

	return SUCCEEDED(hr); // ê¨å˜Ç»ÇÁtrueÇï‘Ç∑ä÷êî
}


/// <summary>
/// Render 
/// </summary>
void TextureSprite::Render(DirectX::SimpleMath::Vector2 pos, float scale, DirectX::FXMVECTOR color)
{
	m_spriteBatch->Begin();

	m_spriteBatch->Draw(m_SRV.Get(),
		pos, nullptr, color, 0.0f, SimpleMath::Vector2(0, 0),
		scale);

	m_spriteBatch->End();
}



/// <summary>
/// Finalize 
/// </summary>
void TextureSprite::Finalize()
{
	m_SRV.Reset();
	m_spriteBatch.reset();
}
