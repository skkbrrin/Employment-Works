#include "pch.h"
#include "PlayScene.h"
#include "ResultScene.h"

using namespace DirectX;

// 敵の初期座標
std::vector<DirectX::SimpleMath::Vector3> enemiesSpawnPos =
{
	DirectX::SimpleMath::Vector3{ 10.0f, 0.0f, 10.0f },
	DirectX::SimpleMath::Vector3{ -8.0f, 0.0f, 10.0f },
	DirectX::SimpleMath::Vector3{ -5.0f, 0.0f,  5.0f },
	DirectX::SimpleMath::Vector3{  7.0f, 0.0f,-10.0f },
	DirectX::SimpleMath::Vector3{-12.0f, 0.0f, -7.0f },
	DirectX::SimpleMath::Vector3{  4.0f, 0.0f, 12.0f },
	DirectX::SimpleMath::Vector3{ 15.0f, 0.0f, -3.0f },
	DirectX::SimpleMath::Vector3{ -9.0f, 0.0f, -12.0f },
	DirectX::SimpleMath::Vector3{  0.0f, 0.0f,  15.0f },
	DirectX::SimpleMath::Vector3{  8.0f, 0.0f,  -6.0f },
};

void PlayScene::Initialize()
{
	CreateDeviceDependentResources();
	CreateWindowSizeDependentResources();

	auto DR = GetUserResources()->GetDeviceResources();
	auto windowSize = GetUserResources()->GetDeviceResources()->GetWindow();

	// スコア関連
	m_attackScore = 0;
	m_timeScore = 0;

	// カウント
	startcount = 4.0f;
	m_countNumber = m_taskManager.AddTask<Number>(&m_spriteBatch, m_numberSRV.GetAddressOf());
	m_countNumber->SetPosition(SimpleMath::Vector2(83.0f, 270.0f));
	m_countNumber->SetNumber(startcount);
	m_countNumber->SetScale(5.0f);

	// タイム関連
	timer = 45.0f;
	m_timeNumber = m_taskManager.AddTask<Number>(&m_spriteBatch, m_numberSRV.GetAddressOf());
	m_timeNumber->SetPosition(SimpleMath::Vector2(350.0f, 0.0f));
	m_timeNumber->SetNumber(timer);
	m_timeNumber->SetScale(2.5f);

	// プレイヤーHPゲージ
	m_hpManager->Initialize(DR);

	// カメラ
	cameraNum = 1;
	m_camera.SetPlayer(m_player.get());

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
	if (kb->pressed.Q || timer <= 0.0f || m_player->GetHP() <= 0 || m_enemies.empty())
	{
		// スコアを事前に加算
		int addScore = static_cast<int>(timer * 5 + 10);
		ScoreManager::Instance().SetTimeScore(
			ScoreManager::Instance().GetTimeScore() + addScore
		);
		ScoreManager::Instance().Update();

		//std::this_thread::sleep_for(std::chrono::seconds{ 1 });
		ChangeScene<ResultScene>();
	}

	// ゲームカメラ
	// 攻撃カメラ----------------------------------------------------



	//---------------------------------------------------------------
	// カメラ更新
	m_camera.Update(elapsedTime);

	// ゲームカウントスタート
	if (startcount > 0)
	{
		startcount -= elapsedTime;
		m_countNumber->SetNumber(startcount);
	}
	// カウント後ゲームスタート
	else if (startcount <= 0)
	{


		// デバッグカメラ
		m_debugCamera->Update();

		// 天球
		float rotateSpeed = 1.0f;
		m_skyRotate += rotateSpeed * elapsedTime;

		if (m_skyRotate > 360.0f)
		{
			m_skyRotate = 0.0f;
		}

		

		// プレイヤー
		/// 攻撃
		if (kb->pressed.Z)
		{
			m_player->NormalAttack(elapsedTime, m_enemies);
		}
		else if (kb->pressed.X)
		{
			m_player->ComboAttack(elapsedTime, m_enemies);
		}

		/// 通常
		m_player->Update(elapsedTime, m_enemy.get());

		// 複数体エネミー
		for (auto& ene : m_enemies)
		{
			ene->Update(elapsedTime, m_player.get());
		}

		for (auto& e : m_enemies)
		{
			if (e->GetIsDie())
			{
				ScoreManager::Instance().SetAttackScore(
					ScoreManager::Instance().GetAttackScore() + 50
				);
			}
		}

		m_enemies.erase(
			std::remove_if(m_enemies.begin(), m_enemies.end(),
				[](const std::unique_ptr<Enemy>& e)
				{
					return e->GetIsDie();
				}),
			m_enemies.end()
		);


		ScoreManager::Instance().Update();

		

		// タイマー
		timer -= elapsedTime;
		m_timeNumber->SetNumber(timer);

		// HPマネージャー
		m_hpManager->Update(m_player->GetHP(), m_player->GetFullHP());

		// シーンチェンジの時に、白い板を画面に出して、透明度を0→１に徐々にしてフェードアウト
		// 白フェードアウト→リザルトバンっとだす。(「大神」常闇之皇戦、戦績風)
	}
}

void PlayScene::Render()
{
	// ビュー行列を設定
	if (cameraNum != 0) {
		m_view = SimpleMath::Matrix::CreateLookAt(
			m_camera.GetEyePosition(),
			m_camera.GetTargetPosition(),
			SimpleMath::Vector3::UnitY
		);
	}

	auto debugFont = GetUserResources()->GetDebugFont();
	auto device = GetUserResources()->GetDeviceResources()->GetD3DDevice();
	auto context = GetUserResources()->GetDeviceResources()->GetD3DDeviceContext();
	auto states = GetUserResources()->GetCommonStates();

	// 床
	m_floorPrimitive->Render(context, m_view, m_proj);

	// 天球-------------------------------------------------------------------------------------------------
	DirectX::SimpleMath::Matrix baseRotation = DirectX::SimpleMath::Matrix::CreateFromAxisAngle(
		DirectX::SimpleMath::Vector3::UnitX, DirectX::XMConvertToRadians(90.0f)) *
		DirectX::SimpleMath::Matrix::CreateFromAxisAngle(
			DirectX::SimpleMath::Vector3::UnitY, DirectX::XMConvertToRadians(180.0f));

	DirectX::SimpleMath::Matrix dynamicRotation = DirectX::SimpleMath::Matrix::CreateFromAxisAngle(
		DirectX::SimpleMath::Vector3::UnitY, DirectX::XMConvertToRadians(m_skyRotate));

	// 天球の最終変換行列
	DirectX::SimpleMath::Matrix im = dynamicRotation * baseRotation;
	im = im * DirectX::SimpleMath::Matrix::CreateFromAxisAngle(DirectX::SimpleMath::Vector3::UnitY, DirectX::XMConvertToRadians(360.0f));

	context->OMSetDepthStencilState(states->DepthNone(), 0);
	context->RSSetState(states->CullNone());

	m_skyModel->UpdateEffects([](IEffect* effect)
		{
			auto lights = dynamic_cast<IEffectLights*>(effect);
			if (lights)
			{
				lights->SetLightEnabled(0, true);

				DirectX::SimpleMath::Vector3 dir(0.0f, 0.0f, 0.0f);  // 逆向きに
				dir.Normalize();
				lights->SetLightDirection(0, DirectX::XMVectorSet(dir.x, dir.y, dir.z, 0.0f));

				lights->SetLightDiffuseColor(0, DirectX::Colors::White);
				lights->SetLightSpecularColor(0, DirectX::Colors::White);
				lights->SetAmbientLightColor(DirectX::Colors::WhiteSmoke);
			}
		});

	m_skyModel->Draw(context, *states, im * SimpleMath::Matrix::CreateScale(9000.0f), m_view, m_proj);
	// 天球-------------------------------------------------------------------------------------------------

	// プレイヤー
	m_player->Render(context, states, m_view, m_proj);

	// エネミー
	for (auto& ene : m_enemies)
	{
		ene->Render(context, states, m_view, m_proj);
	}

	//	スプライトバッチ
	m_spriteBatch->Begin();
	m_countNumber->Render();
	m_timeNumber->Render();
	m_spriteBatch->End();

	// HP
	m_hpManager->Render();
}

void PlayScene::Finalize()
{
	// --- 後始末処理 ---
	for (auto& ene : m_enemies)
	{
		if (ene) ene->Finalize();
	}
	m_enemies.clear();

	if (m_player) { m_player->Finalize(); m_player.reset(); }
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

	m_floorPrimitive = std::make_unique<kHorikawa::FloorPrimitive>(device);

	std::unique_ptr<DirectX::EffectFactory> fx = std::make_unique<DirectX::EffectFactory>(device);
	fx->SetDirectory(L"Resources/Models");

	m_skyModel = DirectX::Model::CreateFromSDKMESH(device, L"Resources/Models/Sky.sdkmesh", *fx);

	m_player = std::make_unique<Player>();
	m_player->Initialize(device);

	/*m_enemy = std::make_unique<Enemy>();
	m_enemy->Inisialize(device);*/

	// 複数体
	for (int i = 0; i < enemiesSpawnPos.size(); i++)
	{
		auto ene = std::make_unique<Enemy>();
		ene->Inisialize(device);

		// 初期値ずらし
		ene->SetPos(enemiesSpawnPos[i]);
		
		m_enemies.push_back(std::move(ene));
	}

	auto device_2D = GetUserResources()->GetDeviceResources()->GetD3DDevice();
	auto context_2D = GetUserResources()->GetDeviceResources()->GetD3DDeviceContext();

	m_spriteBatch = std::make_unique<SpriteBatch>(context_2D);

	DX::ThrowIfFailed(CreateDDSTextureFromFile(device, L"Resources/Textures/number.dds", nullptr, PlayScene::m_numberSRV.ReleaseAndGetAddressOf()));

	m_hpManager = std::make_unique<HPManager>();
	m_hpManager->LoadBase(L"Resources/Textures/base.png");
	m_hpManager->LoadGauge(L"Resources/Textures/gauge.png");
	m_hpManager->LoadFrame(L"Resources/Textures/frame.png");

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
