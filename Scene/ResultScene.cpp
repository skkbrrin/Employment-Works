#include "pch.h"
#include "ResultScene.h"
#include "TitleScene.h"

using namespace DirectX;

void ResultScene::Initialize()
{
	CreateDeviceDependentResources();
	CreateWindowSizeDependentResources();

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
}

void ResultScene::Render()
{
	auto debugFont = GetUserResources()->GetDebugFont();
	debugFont->AddString(L"ResultScene", DirectX::SimpleMath::Vector2(0.0f, debugFont->GetFontHeight()), DirectX::Colors::Black);

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

}

void ResultScene::CreateWindowSizeDependentResources()
{
}

void ResultScene::OnDeviceLost()
{
}
