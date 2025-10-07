#include "pch.h"
#include "ResultScene.h"
#include "TitleScene.h"

using namespace DirectX;

void ResultScene::Initialize()
{
	CreateDeviceDependentResources();
	CreateWindowSizeDependentResources();

	ScoreManager::Instance().Update();

	attack = ScoreManager::Instance().GetAttackScore();
	time = ScoreManager::Instance().GetTimeScore();
	total = ScoreManager::Instance().GetTotalScore();

	wchar_t buf[256];
	swprintf(buf, 256, L"[Initialize] attack=%d time=%d total=%d\n",
		ScoreManager::Instance().GetAttackScore(),
		ScoreManager::Instance().GetTimeScore(),
		ScoreManager::Instance().GetTotalScore());
	OutputDebugString(buf);

	// 討伐得点
	m_attackNumber = m_taskManager.AddTask<Number>(&m_spriteBatch, m_numberSRV.GetAddressOf());
	m_attackNumber->SetNumber(attack);

	// 余時点
	m_timeNumber = m_taskManager.AddTask<Number>(&m_spriteBatch, m_numberSRV.GetAddressOf());
	m_timeNumber->SetNumber(time);

	// 総合得点
	m_totalNumber = m_taskManager.AddTask<Number>(&m_spriteBatch, m_numberSRV.GetAddressOf());
	m_totalNumber->SetNumber(total);

	// 座標とスケールは最後に設定
	m_attackNumber->SetPosition({ 600.0f, 260.0f });
	m_attackNumber->SetScale(2.0f);

	m_timeNumber->SetPosition({ 600.0f, 380.0f });
	m_timeNumber->SetScale(2.0f);

	m_totalNumber->SetPosition({ 500.0f, 530.0f });
	m_totalNumber->SetScale(3.0f);


	// BGM
	AUDIO_ENGINE_FLAGS flags = AudioEngine_Default;
	m_audioE = std::make_unique<AudioEngine>(flags);

	try {
		m_bgm = std::make_unique<SoundEffect>(m_audioE.get(), L"Resources/Sounds/リザルトBGM.wav");
		m_bgmInstance = m_bgm->CreateInstance();
		m_bgmInstance->Play(false);
	}
	catch (const std::exception& e) {
		OutputDebugStringA(e.what());
	}
}


void ResultScene::Update(float elapsedTime)
{
	auto kb = DirectX::Keyboard::Get().GetState();
	auto kbState = GetUserResources()->GetKeyboardStateTracker();

	if (kbState->pressed.Enter)
	{
		ChangeScene<TitleScene>();
	}

	m_textureAlpha->Update(elapsedTime);
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
	//m_spriteBatch->Draw(m_rankSRV.Get(), SimpleMath::Vector2(0.0f, 0.0f), nullptr);
	m_spriteBatch->End();

	m_textureAlpha->Render({ 1150.0f, 560.0f }, 0.4f);

	m_spriteBatch = std::make_unique<SpriteBatch>(
		GetUserResources()->GetDeviceResources()->GetD3DDeviceContext()
	);

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

	m_textureSprite = std::make_unique<TextureSprite>(device, context);
	m_textureSprite->Load(L"Resources/Textures/Result.dds");

	m_textureAlpha = std::make_unique<TextureAlpha>(device, context);
	m_textureAlpha->Load(L"Resources/Textures/OnryEnter.dds");

	m_spriteBatch = std::make_unique<SpriteBatch>(context);

	DX::ThrowIfFailed(CreateDDSTextureFromFile(device, L"Resources/Textures/number.dds", nullptr, ResultScene::m_numberSRV.ReleaseAndGetAddressOf()));

	// ランク
	/*if (m_scoreManager->GetTotalScore() <= 200)
	{
		DX::ThrowIfFailed(CreateDDSTextureFromFile(device, L"Resources/Textures/retu.dds", nullptr, ResultScene::m_rankSRV.ReleaseAndGetAddressOf()));
	}
	else if(m_scoreManager->GetTotalScore() <= 800)
	{
		DX::ThrowIfFailed(CreateDDSTextureFromFile(device, L"Resources/Textures/hei.dds", nullptr, ResultScene::m_rankSRV.ReleaseAndGetAddressOf()));
	}
	else
	{
		DX::ThrowIfFailed(CreateDDSTextureFromFile(device, L"Resources/Textures/Yu.dds", nullptr, ResultScene::m_rankSRV.ReleaseAndGetAddressOf()));
	}*/
}

void ResultScene::CreateWindowSizeDependentResources()
{
}

void ResultScene::OnDeviceLost()
{
}
