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

	// カメラ
	//m_camera.SetPlayer(&m_player->m_position, &m_player->m_rotation);

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
	auto device = GetUserResources()->GetDeviceResources()->GetD3DDevice();

	// リザルト切り替え条件
	if (kb->pressed.Q)
	{
		ChangeScene<ResultScene>();
	}

	// ゲームカメラ
	// 攻撃カメラ----------------------------------------------------



	//---------------------------------------------------------------
	// カメラ更新
	//デバッグカメラ
	m_debugCamera->Update();
	//m_camera.Update(elapsedTime);

	// フィールド
	m_field->Update(elapsedTime);

	//タイマー

	//HPマネージャー

	//シーンチェンジの時に、白い板を画面に出して、透明度を0→１に徐々にしてフェードアウト
	//白フェードアウト→リザルトバンっとだす。(「大神」常闇之皇戦、戦績風)
	
}

void PlayScene::Render()
{
	// ビュー行列を設定
	auto debugFont = GetUserResources()->GetDebugFont();
	auto device = GetUserResources()->GetDeviceResources()->GetD3DDevice();
	auto context = GetUserResources()->GetDeviceResources()->GetD3DDeviceContext();
	auto states = GetUserResources()->GetCommonStates();

	m_view = m_debugCamera->GetCameraMatrix();
	/*m_view = SimpleMath::Matrix::CreateLookAt(
		m_camera.GetEyePosition(),
		m_camera.GetTargetPosition(),
		SimpleMath::Vector3::UnitY
	);*/

	// 床
	m_floorPrimitive->Render(context, m_view, m_proj);

	// 天球
	SimpleMath::Matrix world_S = SimpleMath::Matrix::Identity;
	world_S = SimpleMath::Matrix::CreateScale(9000);
	m_skyModel->Draw(context, *states,world_S, m_view, m_proj);

	// フィールド
	m_field->Render(context, states, m_view, m_proj);
}

void PlayScene::Finalize()
{
}

void PlayScene::CreateDeviceDependentResources()
{
	//std::this_thread::sleep_for(std::chrono::seconds{ 2 });

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

	// 床
	m_floorPrimitive = std::make_unique<kHorikawa::FloorPrimitive>(device);

	std::unique_ptr<DirectX::EffectFactory> fx = std::make_unique<DirectX::EffectFactory>(device);
	fx->SetDirectory(L"Resources/Models");

	// 天球
	m_skyModel = DirectX::Model::CreateFromSDKMESH(device, L"Resources/Models/Sky.sdkmesh", *fx);

	// フィールド
	m_field = std::make_unique<Field>();
	m_field->Initialize(device, context);
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
