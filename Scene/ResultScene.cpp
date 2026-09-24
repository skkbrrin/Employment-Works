#include "pch.h"
#include "ResultScene.h"
#include "TitleScene.h"
#include "ResourceManager/Resource.h"

#include "WoodManager.h"

using namespace DirectX;

// 初期化
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

	Resource::Texture().Load(L"Resources/Textures/Result.png");

	// 数字
	m_number = m_taskManager.AddTask<Number>(&m_spriteBatch, m_numberSRV.GetAddressOf());
	m_number->SetNumber(WoodManager::Instance().Get());
	m_number->SetScale(4.5f);
	m_number->SetPosition(DirectX::SimpleMath::Vector2(150.0f, 230.0f));
}

// 更新
void ResultScene::Update(float elapsedTime)
{
	auto kb = DirectX::Keyboard::Get().GetState();
	auto kbState = GetUserResources()->GetKeyboardStateTracker();

	m_number->Update(elapsedTime);
	m_taskManager.Update(elapsedTime);

	// シーンチェンジ
	if (kbState->pressed.Space)
	{
		ChangeScene<TitleScene>();
	}
}

// 描画
void ResultScene::Render()
{
	auto debugFont = GetUserResources()->GetDebugFont();
	debugFont->AddString(L"ResultScene", DirectX::SimpleMath::Vector2(0.0f, debugFont->GetFontHeight()), DirectX::Colors::Black);

	// 画面としては、スコア数値以外の物が書かれているテクスチャ一枚
	m_spriteBatch->Begin();

	m_spriteBatch->Draw(
		m_backTexture.Get(),
		DirectX::XMFLOAT2(0, 0), // 位置
		nullptr,                    // 切り抜き
		DirectX::Colors::White,     // 色（Whiteで元の色）
		0.0f,                       // 回転
		DirectX::XMFLOAT2(0, 0),    // 原点
		1.0f                        // 拡大率
	);

	m_spriteBatch->Draw(
		m_textTexture.Get(),
		DirectX::XMFLOAT2(100, 0), // 位置
		nullptr,                    // 切り抜き
		DirectX::Colors::White,     // 色（Whiteで元の色）
		0.0f,                       // 回転
		DirectX::XMFLOAT2(0, 0),    // 原点
		0.5f                        // 拡大率
	);

	m_spriteBatch->Draw(
		m_textTexture_02.Get(),
		DirectX::XMFLOAT2(650, 150), // 位置
		nullptr,                    // 切り抜き
		DirectX::Colors::White,     // 色（Whiteで元の色）
		0.0f,                       // 回転
		DirectX::XMFLOAT2(0, 0),    // 原点
		0.5f                        // 拡大率
	);

	m_taskManager.Render();

	m_spriteBatch->End();
	//               →スコア
	// の順番に貼っていく
	// 一番右下にpress enterの文字をタイトルと同じように描画
}

// 終了
void ResultScene::Finalize()
{
}

// デバイスリソース
void ResultScene::CreateDeviceDependentResources()
{
	auto device = GetUserResources()->GetDeviceResources()->GetD3DDevice();
	auto context = GetUserResources()->GetDeviceResources()->GetD3DDeviceContext();

	m_spriteBatch = std::make_unique<SpriteBatch>(context);

	DirectX::CreateWICTextureFromFile(
		device,
		L"Resources/Textures/Text.png",
		nullptr,
		m_textTexture.ReleaseAndGetAddressOf()
	);

	DirectX::CreateWICTextureFromFile(
		device,
		L"Resources/Textures/Text02.png",
		nullptr,
		m_textTexture_02.ReleaseAndGetAddressOf()
	);

	DirectX::CreateWICTextureFromFile(
		device,
		L"Resources/Textures/Result.png",
		nullptr,
		m_backTexture.ReleaseAndGetAddressOf()
	);

	(DirectX::CreateWICTextureFromFile(
		device,
		L"Resources/Textures/number.png",
		nullptr,
		m_numberSRV.ReleaseAndGetAddressOf()
	));

}

void ResultScene::CreateWindowSizeDependentResources()
{
}

void ResultScene::OnDeviceLost()
{
}
