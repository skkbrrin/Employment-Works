#include "pch.h"
#include "TitleScene.h"
#include "Scene/LoadingScreen.h"
#include "Scene/PlayScene.h"

#include <CommonStates.h>

#include "WoodManager.h"

using namespace DirectX;

void TitleScene::Initialize()
{
	CreateDeviceDependentResources();
	CreateWindowSizeDependentResources();

	// BGM
	AUDIO_ENGINE_FLAGS flags = AudioEngine_Default;
	audioEngine = std::make_unique<AudioEngine>(flags);

	try {
		bgm = std::make_unique<SoundEffect>(audioEngine.get(), L"Resources/Sounds/Momiziyado.wav");
		bgmInstance = bgm->CreateInstance();
		bgmInstance->Play(true);
	}
	catch (const std::exception& e) {
		OutputDebugStringA(e.what());
	}

	WoodManager::Instance().Reset();
}

void TitleScene::Update(float elapsedTime)
{
	UNREFERENCED_PARAMETER(elapsedTime);

	auto kb = GetUserResources()->GetKeyboardStateTracker();

	if (kb->pressed.Space)
	{
		bgmInstance->Stop();
		ChangeScene<PlayScene, LoadingScreen>();
	}

	m_tectureAlpha->Update(elapsedTime);
}

void TitleScene::Render()
{
	auto device = GetUserResources()->GetDeviceResources()->GetD3DDevice();
	auto states = GetUserResources()->GetCommonStates();

	m_textureSprite->Render({ 0.0f, 0.0f });
	m_tectureAlpha->Render({ 320.0f, 450.0f });
}

void TitleScene::Finalize()
{
	// I—¹ˆ—
	bgmInstance.reset();
	bgm.reset();
	audioEngine.reset();

}

void TitleScene::CreateDeviceDependentResources()
{
	auto device = GetUserResources()->GetDeviceResources()->GetD3DDevice();
	auto context = GetUserResources()->GetDeviceResources()->GetD3DDeviceContext();

	m_textureSprite = std::make_unique<TextureSprite>(device, context);

	m_textureSprite->Load(L"Resources/Textures/Title.png");

	m_tectureAlpha = std::make_unique<TextureAlpha>(device, context);
	m_tectureAlpha->Load(L"Resources/Textures/Space.png");
}

void TitleScene::CreateWindowSizeDependentResources()
{
}

void TitleScene::OnDeviceLost()
{
}