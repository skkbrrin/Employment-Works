/*
タイトルシーン
*/

#include "pch.h"

#include "LoadingScreen.h"

#include <WICTextureLoader.h>
#include <SimpleMath.h>

using namespace DirectX;



/*--------------------------------------------------
コンストラクタ
--------------------------------------------------*/
LoadingScreen::LoadingScreen()
{
}

/*--------------------------------------------------
デストラクタ
--------------------------------------------------*/
LoadingScreen::~LoadingScreen()
{
}

/*--------------------------------------------------
初期化
--------------------------------------------------*/
void LoadingScreen::Initialize()
{
	CreateDeviceDependentResources();
	CreateWindowSizeDependentResources();
}

/*--------------------------------------------------
更新
戻り値	:次のシーン番号
--------------------------------------------------*/
void LoadingScreen::Update(float elapsedTime)
{
	m_rotation += elapsedTime;
}

/*--------------------------------------------------
描画
--------------------------------------------------*/
void LoadingScreen::Render()
{
	m_spriteBatch->Begin();
	m_spriteBatch->Draw(m_backSRV.Get(), SimpleMath::Vector2(0.0f, 0.0f), nullptr, DirectX::XMVECTOR{1.0f, 1.0f, 1.0f, 0.5f}, 0.0f, SimpleMath::Vector2(0.0f, 0.0f), 1.0f, SpriteEffects_None);
	m_spriteBatch->Draw(m_SRV.Get(), SimpleMath::Vector2(1150.0f, 600.0f), nullptr, Colors::White, m_rotation, SimpleMath::Vector2(190.0f, 190.0f), 0.5f, SpriteEffects_None);
	m_spriteBatch->Draw(m_moziSRV.Get(), SimpleMath::Vector2(750.0f, 550.0f), nullptr, Colors::White, 0.0f, SimpleMath::Vector2(190.0f, 190.0f), 0.3f, SpriteEffects_None);
	m_spriteBatch->End();
}

/*--------------------------------------------------
終了処理
--------------------------------------------------*/
void LoadingScreen::Finalize()
{
}

void LoadingScreen::CreateDeviceDependentResources()
{
	auto device = GetUserResources()->GetDeviceResources()->GetD3DDevice();
	auto context = GetUserResources()->GetDeviceResources()->GetD3DDeviceContext();

	m_spriteBatch = std::make_unique<SpriteBatch>(context);

	CreateDDSTextureFromFile(device, L"Resources/Textures/mitudomoe.dds", nullptr, LoadingScreen::m_SRV.ReleaseAndGetAddressOf());
	CreateDDSTextureFromFile(device, L"Resources/Textures/back.dds", nullptr, LoadingScreen::m_backSRV.ReleaseAndGetAddressOf());
	CreateDDSTextureFromFile(device, L"Resources/Textures/Yomikomi.dds", nullptr, LoadingScreen::m_moziSRV.ReleaseAndGetAddressOf());

}

void LoadingScreen::CreateWindowSizeDependentResources()
{
}

void LoadingScreen::OnDeviceLost()
{
	Finalize();
}

