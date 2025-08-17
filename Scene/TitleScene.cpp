#include "pch.h"
#include "TitleScene.h"
#include "Scene/LoadingScreen.h"
#include "Scene/PlayScene.h"

#include <CommonStates.h>


using namespace DirectX;

void TitleScene::Initialize()
{
	CreateDeviceDependentResources();
	CreateWindowSizeDependentResources();

	// BGM
	AUDIO_ENGINE_FLAGS flags = AudioEngine_Default;
#ifdef _DEBUG
	flags |= AudioEngine_Debug;
#endif

	audioEngine = std::make_unique<AudioEngine>(flags);

	try {
		bgm = std::make_unique<SoundEffect>(audioEngine.get(), L"Resources/Sounds/Momiziyado.wav");
		bgmInstance = bgm->CreateInstance();
		bgmInstance->Play(true);
	}
	catch (const std::exception& e) {
		OutputDebugStringA(e.what());
	}
}

void TitleScene::Update(float elapsedTime)
{
	UNREFERENCED_PARAMETER(elapsedTime);

	auto kb = GetUserResources()->GetKeyboardStateTracker();

	if (kb->pressed.Enter)
	{
		bgmInstance->Stop();
		ChangeScene<PlayScene, LoadingScreen>();
	}

	// 毎フレーム更新
	timer += elapsedTime;

	// アルファ値を 0～1 の範囲で変化させる（点滅）
	float alpha = abs(sinf(timer)); // speedで速さ調整
	color = { alpha, alpha, alpha, alpha };
}

void TitleScene::Render()
{
	auto device = GetUserResources()->GetDeviceResources()->GetD3DDevice();
	auto states = GetUserResources()->GetCommonStates();

	m_spriteBatch->Begin(SpriteSortMode_Deferred,
		states->AlphaBlend());
	m_spriteBatch->Draw(m_SRV.Get(), SimpleMath::Vector2(0.0f, 0.0f), nullptr, Colors::White, 0.0f, SimpleMath::Vector2(0.0f, 0.0f), 0.92f, SpriteEffects_None);
	m_spriteBatch->Draw(m_callSRV.Get(), SimpleMath::Vector2(500.0f, 500.0f), nullptr, color, 0.0f, SimpleMath::Vector2(0.0f, 0.0f), 0.5f, SpriteEffects_None);
	m_spriteBatch->End();
}

void TitleScene::Finalize()
{
	// 終了処理
	bgmInstance.reset();
	bgm.reset();
	audioEngine.reset();
}

void TitleScene::CreateDeviceDependentResources()
{
	auto device = GetUserResources()->GetDeviceResources()->GetD3DDevice();
	auto context = GetUserResources()->GetDeviceResources()->GetD3DDeviceContext();

	m_spriteBatch = std::make_unique<SpriteBatch>(context);

	CreateDDSTextureFromFile(device, L"Resources/Textures/Title.dds", nullptr, TitleScene::m_SRV.ReleaseAndGetAddressOf());
	CreateDDSTextureFromFile(device, L"Resources/Textures/Enter.dds", nullptr, TitleScene::m_callSRV.ReleaseAndGetAddressOf());
}

void TitleScene::CreateWindowSizeDependentResources()
{
}

void TitleScene::OnDeviceLost()
{
}