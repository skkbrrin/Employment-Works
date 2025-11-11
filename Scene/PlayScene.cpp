#include "pch.h"
#include "PlayScene.h"
#include "ResultScene.h"

using namespace DirectX;

void PlayScene::Initialize()
{
	CreateDeviceDependentResources();
	CreateWindowSizeDependentResources();

	auto DR = GetUserResources()->GetDeviceResources();
	auto windowSize = GetUserResources()->GetDeviceResources()->GetWindow();

	// BGM
	AUDIO_ENGINE_FLAGS flags = AudioEngine_Default;
	m_audioE = std::make_unique<AudioEngine>(flags);

	try {
		m_bgm = std::make_unique<SoundEffect>(m_audioE.get(), L"Resources/Sounds/毘沙門.wav");
		m_bgmInstance = m_bgm->CreateInstance();
		m_bgmInstance->Play(true);
	}
	catch (const std::exception& e) {
		OutputDebugStringA(e.what());
	}
}

void PlayScene::Update(float elapsedTime)
{
	auto kb = GetUserResources()->GetKeyboardStateTracker();

	// リザルト切り替え条件
	

	// ゲームカメラ
	// 攻撃カメラ----------------------------------------------------



	//---------------------------------------------------------------
	// カメラ更新

	// ゲームカウントスタート
	// カウント後ゲームスタート

		// デバッグカメラ

		// 天球

		// プレイヤー

		// 複数体エネミー

		// タイマー

		// HPマネージャー
}

void PlayScene::Render()
{
	// ビュー行列を設定

	auto debugFont = GetUserResources()->GetDebugFont();
	auto device = GetUserResources()->GetDeviceResources()->GetD3DDevice();
	auto context = GetUserResources()->GetDeviceResources()->GetD3DDeviceContext();
	auto states = GetUserResources()->GetCommonStates();

	// 床

	// 天球

	// プレイヤー

	// エネミー

	//	スプライトバッチ
	m_spriteBatch->Begin();
	m_spriteBatch->End();

	// HP
}

void PlayScene::Finalize()
{
	// --- 後始末処理 ---
}

void PlayScene::CreateDeviceDependentResources()
{
	std::this_thread::sleep_for(std::chrono::seconds{ 2 });

	auto device = GetUserResources()->GetDeviceResources()->GetD3DDevice();
	auto context = GetUserResources()->GetDeviceResources()->GetD3DDeviceContext();
	auto state = GetUserResources()->GetCommonStates();

	// ベーシックエフェクトの作成
	m_basicEffect = std::make_unique<BasicEffect>(device);
	m_basicEffect->SetLightingEnabled(true);
	m_basicEffect->SetVertexColorEnabled(false);
	m_basicEffect->SetTextureEnabled(true);

	// プリミティブバッチの作成
	m_primitiveBatch = std::make_unique<PrimitiveBatch<VertexPositionTexture>>(context);

	std::unique_ptr<DirectX::EffectFactory> fx = std::make_unique<DirectX::EffectFactory>(device);
	fx->SetDirectory(L"Resources/Models");

	m_skyModel = DirectX::Model::CreateFromSDKMESH(device, L"Resources/Models/Sky.sdkmesh", *fx);


	auto device_2D = GetUserResources()->GetDeviceResources()->GetD3DDevice();
	auto context_2D = GetUserResources()->GetDeviceResources()->GetD3DDeviceContext();

	m_spriteBatch = std::make_unique<SpriteBatch>(context_2D);
}

void PlayScene::CreateWindowSizeDependentResources()
{
	// 射影行列を作成
	RECT rect = GetUserResources()->GetDeviceResources()->GetOutputSize();
	m_proj = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(
		DirectX::XMConvertToRadians(45.0f),
		static_cast<float>(rect.right) / static_cast<float>(rect.bottom),
		0.1f, 10000.0f
	);

	// デバッグカメラの作成
	m_debugCamera = std::make_unique<Ito::DebugCamera>(rect.right, rect.bottom);
}

void PlayScene::OnDeviceLost()
{
}
