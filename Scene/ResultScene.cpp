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

	m_number = m_taskManager.AddTask<Number>(&m_spriteBatch, m_numberSRV.GetAddressOf());
	//m_number->SetNumber(m_scoreManager->GetTotalScore());
	m_number->SetNumber(9999999);
	m_number->SetPosition(SimpleMath::Vector2(300.0f, 50.0f));
	m_number->SetScale(3.0f);

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


	/*std::wostringstream AtS;
	AtS << "AttackScore : " << m_scoreManager->GetAttackScoer();
	debugFont->AddString(AtS.str().c_str(), SimpleMath::Vector2(450.0f, debugFont->GetFontHeight() * 2), DirectX::Colors::Black);
	std::wostringstream TiS;
	TiS << "TimeScore : " << m_scoreManager->GetTimeScore();
	debugFont->AddString(TiS.str().c_str(), SimpleMath::Vector2(450.0f, debugFont->GetFontHeight() * 3), DirectX::Colors::Black);
	debugFont->AddString(L"--------------------------", DirectX::SimpleMath::Vector2(420.0f, debugFont->GetFontHeight() * 4), DirectX::Colors::Black);
	std::wostringstream ToS;
	ToS << "TotalScore : " << m_scoreManager->GetTotalScore();
	debugFont->AddString(ToS.str().c_str(), SimpleMath::Vector2(450.0f, debugFont->GetFontHeight() * 5), DirectX::Colors::Black);*/

	m_spriteBatch->Begin();
	m_number->Render();
	m_taskManager.Render();
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
	m_spriteBatch = std::make_unique<SpriteBatch>(context);

	DX::ThrowIfFailed(CreateDDSTextureFromFile(device, L"Resources/Textures/number.dds", nullptr, ResultScene::m_numberSRV.ReleaseAndGetAddressOf()));
}

void ResultScene::CreateWindowSizeDependentResources()
{
}

void ResultScene::OnDeviceLost()
{
}
