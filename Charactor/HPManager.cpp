//-------------------------------------------------------------------------------------
// File: HPManager.h
//
// render floor by primitive
//
//-------------------------------------------------------------------------------------

#include "pch.h"
#include "HPManager.h"
#include <PrimitiveBatch.h> 
#include <VertexTypes.h> 
#include <WICTextureLoader.h> 

using namespace DirectX;

// 指定範囲内に収めるテンプレート
template <typename T>
T Clamp(const T& value, const T& min, const T& max)
{
	return (value < min) ? min : (value > max ? max : value);
}

/// <summary>
/// Constractor
/// </summary>
HPManager::HPManager(ID3D11Device1* device, ID3D11DeviceContext* context)
	: m_context(context)
	, m_device(device)
	, m_color()
{
	m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(context);
}

/// <summary>
/// Destractor
/// </summary>
HPManager::~HPManager()
{
	Finalize();
}

bool HPManager::Load(const wchar_t* back, const wchar_t* fill)
{
	HRESULT hr_1 = CreateDDSTextureFromFile(
		m_device,
		back,
		nullptr,
		m_backSRV.ReleaseAndGetAddressOf());

	HRESULT hr_2 = CreateDDSTextureFromFile(
		m_device,
		fill,
		nullptr,
		m_fillSRV.ReleaseAndGetAddressOf());

	return SUCCEEDED(hr_1) && SUCCEEDED(hr_2); // 成功ならtrueを返す関数
}

void HPManager::Update(int HP, int maxHP)
{
	if (maxHP <= 0) maxHP = 1; // ０で割ってエラーが出るのを防止
	m_HP = Clamp((float)HP / (float)maxHP, 0.0f, 1.0f);
}


/// <summary>
/// Render 
/// </summary>
void HPManager::Render(DirectX::SimpleMath::Vector2 pos, float scale)
{
	m_spriteBatch->Begin();

	// 枠部分
	m_spriteBatch->Draw(m_backSRV.Get(),
		pos, nullptr, m_color, 0.0f, SimpleMath::Vector2(0, 0),
		scale);

	// 中身部分
	RECT rect = { 0, 0,(LONG)(m_fillW * m_HP), m_fillH };

	m_spriteBatch->Draw(m_fillSRV.Get(),
		pos, &rect, m_color, 0.0f, SimpleMath::Vector2(0, 0),
		scale);

	m_spriteBatch->End();
}



/// <summary>
/// Finalize 
/// </summary>
void HPManager::Finalize()
{
	m_backSRV.Reset();
	m_fillSRV.Reset();
	m_spriteBatch.reset();
}
