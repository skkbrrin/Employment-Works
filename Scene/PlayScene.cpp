#include "pch.h"
#include "PlayScene.h"
#include "ResultScene.h"
#include "WoodManager.h"

using namespace DirectX;


void PlayScene::Initialize()
{
	CreateDeviceDependentResources();
	CreateWindowSizeDependentResources();

	auto DR = GetUserResources()->GetDeviceResources();
	auto windowSize = GetUserResources()->GetDeviceResources()->GetWindow();
	auto WH = GetUserResources()->GetDeviceResources()->GetScreenViewport();
	auto device = GetUserResources()->GetDeviceResources()->GetD3DDevice();
	auto context = GetUserResources()->GetDeviceResources()->GetD3DDeviceContext();

	m_field->Initialize(device, context, DR);

	// カメラ
	m_camera.SetPlayer(&m_field->GetPlayer()->GetPosition(), &m_field->GetPlayer()->GetRotation());

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

	m_number = m_taskManager.AddTask<Number>(&m_spriteBatch, m_numberSRV.GetAddressOf());
	m_number->SetNumber(WoodManager::Instance().Get());
	m_number->SetScale(2.0f);
	m_number->SetPosition(DirectX::SimpleMath::Vector2(450.0f, 650.0f));

	m_timerUI->Initialize(DR, (int)WH.Width, (int)WH.Height);
}

void PlayScene::Update(float elapsedTime)
{
	auto kb = GetUserResources()->GetKeyboardStateTracker();
	auto device = GetUserResources()->GetDeviceResources()->GetD3DDevice();

	m_number->Update(elapsedTime);
	m_number->SetNumber(WoodManager::Instance().Get());
	m_taskManager.Update(elapsedTime);

	// リザルト切り替え条件
	if (kb->pressed.Q || m_field->GetPlayer()->GetHP() <= 0 || m_timerUI->IsTimeUp())
	{
		ChangeScene<ResultScene>();
	}

	// カメラ更新
	//デバッグカメラ
	m_debugCamera->Update();
	m_camera.Update(elapsedTime);

	switch (m_field->GetPlayer()->GetCameraRequest())
	{
	case CameraRequest::SpinPrepare:
		m_camera.SetOffset(
			m_field->GetPlayer()->GetRight() * 1.5f + DirectX::SimpleMath::Vector3(0, 7.0f, -3.0f),
			DirectX::SimpleMath::Vector3(0.0f, 6.5f, -5.0f)                     
		);
		break;

	case CameraRequest::SpinMain:
		m_camera.SetOffset(
			DirectX::SimpleMath::Vector3(0.0f, 7.0f, -6.5f),
			DirectX::SimpleMath::Vector3(0.0f, 6.5f, -5.0f) 
		);
		break;

	case CameraRequest::SpinEnd:
		m_camera.Reset();
		break;
	}

	// フィールド
	m_field->Update(elapsedTime);

	//タイマー
	m_timerUI->Update(elapsedTime);

	

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
	if (!m_field->GetPlayer()->GetPowerAttacking())
	{
		m_view = SimpleMath::Matrix::CreateLookAt(
			m_camera.GetEyePosition(),
			m_camera.GetTargetPosition(),
			SimpleMath::Vector3::UnitY
		);
	}
	else
	{
		auto playerPos = m_field->GetPlayer()->GetPosition();

		// プレイヤーの位置からのカメラオフセット
		SimpleMath::Vector3 cameraOffset(0.0f, 10.0f, -30.0f); // Yが上方向、Zが後ろ方向
		SimpleMath::Vector3 eyePos = playerPos + cameraOffset;

		m_view = SimpleMath::Matrix::CreateLookAt(
			eyePos,
			playerPos, // 注視点はプレイヤー
			SimpleMath::Vector3::UnitY
		);
	}

	// 床
	m_floorPrimitive->Render(context, m_view, m_proj);

	// 天球
	SimpleMath::Matrix world_S = SimpleMath::Matrix::Identity;
	world_S = SimpleMath::Matrix::CreateScale(110);
	m_skyModel->Draw(context, *states,world_S, m_view, m_proj);

	// フィールド
	m_field->Render(context, states, m_view, m_proj);

	// HP　タイマー
	m_timerUI->Render();

	m_spriteBatch->Begin();
	m_hp->Render(m_spriteBatch.get(), m_field->GetPlayer()->GetHP(), m_field->GetPlayer()->GetMaxHP());
	m_taskManager.Render();
	m_spriteBatch->Draw(
		m_woodTexture.Get(),
		DirectX::XMFLOAT2(520, 650), // 位置
		nullptr,                    // 切り抜き
		DirectX::Colors::White,     // 色（Whiteで元の色）
		0.0f,                       // 回転
		DirectX::XMFLOAT2(0, 0),    // 原点
		0.13f                        // 拡大率
	);
	m_spriteBatch->End();

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
	auto DR = GetUserResources()->GetDeviceResources();

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
	m_field->Initialize(device, context, DR);

	// スプライトバッチ
	m_spriteBatch = std::make_unique<SpriteBatch>(context);

	// HP
	m_hp = std::make_unique<HPUI>();
	m_hp->Initialize(device, context, L"Resources/Textures/Tomoe.png");

	// タイマー
	m_timerUI = std::make_unique<TimerUI>();

	DirectX::CreateWICTextureFromFile(
		device,
		L"Resources/Textures/numberW.png",
		nullptr,
		m_numberSRV.ReleaseAndGetAddressOf()
	);

	DirectX::CreateWICTextureFromFile(
		device,
		L"Resources/Textures/Wood.png",
		nullptr,
		m_woodTexture.ReleaseAndGetAddressOf()
	);
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
