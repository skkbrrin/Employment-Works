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
		m_bgm = std::make_unique<SoundEffect>(m_audioE.get(), L"Resources/Sounds/ƒŠƒUƒ‹ƒgBGM.wav");
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
	m_spriteBatch->End();

	m_textureAlpha->Render({ 1150.0f, 560.0f }, 0.4f);

	m_spriteBatch = std::make_unique<SpriteBatch>(
		GetUserResources()->GetDeviceResources()->GetD3DDeviceContext()
	);
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

}

void ResultScene::CreateWindowSizeDependentResources()
{
}

void ResultScene::OnDeviceLost()
{
}
