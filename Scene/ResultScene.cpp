#include "pch.h"
#include "ResultScene.h"
#include "TitleScene.h"

using namespace DirectX;

void ResultScene::Initialize()
{
	CreateDeviceDependentResources();
	CreateWindowSizeDependentResources();

	m_scoreManager->Initialize();
	m_scoreManager->Update();

	// 討伐得点
	m_attackNumber = m_taskManager.AddTask<Number>(&m_spriteBatch, m_numberSRV.GetAddressOf());
	m_attackNumber->SetNumber(m_scoreManager->GetAttackScore());
	m_attackNumber->SetPosition(SimpleMath::Vector2(600.0f, 260.0f));
	m_attackNumber->SetScale(2.0f);

	// 余時点
	m_timeNumber = m_taskManager.AddTask<Number>(&m_spriteBatch, m_numberSRV.GetAddressOf());
	m_timeNumber->SetNumber(m_scoreManager->GetTimeScore());
	m_timeNumber->SetPosition(SimpleMath::Vector2(600.0f, 380.0f));
	m_timeNumber->SetScale(2.0f);

	// 総合得点
	m_totalNumber = m_taskManager.AddTask<Number>(&m_spriteBatch, m_numberSRV.GetAddressOf());
	m_totalNumber->SetNumber(m_scoreManager->GetTotalScore());
	m_totalNumber->SetPosition(SimpleMath::Vector2(530.0f, 530.0f));
	m_totalNumber->SetScale(3.0f);

}

void ResultScene::Update(float elapsedTime)
{
	auto kb = DirectX::Keyboard::Get().GetState();
	auto kbState = GetUserResources()->GetKeyboardStateTracker();

	if (kbState->pressed.Enter)
	{
		ChangeScene<TitleScene>();
	}

}

void ResultScene::Render()
{
	auto debugFont = GetUserResources()->GetDebugFont();
	debugFont->AddString(L"ResultScene", DirectX::SimpleMath::Vector2(0.0f, debugFont->GetFontHeight()), DirectX::Colors::Black);

	m_textureSprite->Render({ 0.0f, 0.0f });

	m_spriteBatch->Begin();
	m_attackNumber->Render();
	m_timeNumber->Render();
	m_totalNumber->Render();
	m_spriteBatch->End();

	// 画面としては、スコア数値以外の物が書かれているテクスチャ一枚
	//               →スコア
	// の順番に貼っていく
	// 一番右下にpress enterの文字をタイトルと同じように描画
}

void ResultScene::Finalize()
{
}

void ResultScene::CreateDeviceDependentResources()
{
	auto device = GetUserResources()->GetDeviceResources()->GetD3DDevice();
	auto context = GetUserResources()->GetDeviceResources()->GetD3DDeviceContext();

	m_scoreManager = std::make_unique <ScoreManager>();
	
	m_textureSprite = std::make_unique<TextureSprite>(device, context);
	m_textureSprite->Load(L"Resources/Textures/Result.dds");

	m_spriteBatch = std::make_unique<SpriteBatch>(context);

	DX::ThrowIfFailed(CreateDDSTextureFromFile(device, L"Resources/Textures/number.dds", nullptr, ResultScene::m_numberSRV.ReleaseAndGetAddressOf()));
}

void ResultScene::CreateWindowSizeDependentResources()
{
}

void ResultScene::OnDeviceLost()
{
}
