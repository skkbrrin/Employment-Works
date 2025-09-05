#include "pch.h"
#include "PlayScene.h"
#include "ResultScene.h"

using namespace DirectX;

void PlayScene::Initialize()
{
	CreateDeviceDependentResources();
	CreateWindowSizeDependentResources();

	cameraNum = 1;
	timer = 45.0f;

	m_timeNumber = m_taskManager.AddTask<Number>(&m_spriteBatch, m_numberSRV.GetAddressOf());
	m_timeNumber->SetPosition(SimpleMath::Vector2(350.0f, 0.0f));
	m_timeNumber->SetNumber(timer);
	m_timeNumber->SetScale(2.5f);
}

void PlayScene::Update(float elapsedTime)
{
	auto kb = GetUserResources()->GetKeyboardStateTracker();

	m_debugCamera->Update();


	float rotateSpeed = 1.0f;
	m_skyRotate += rotateSpeed * elapsedTime;

	if (m_skyRotate > 360.0f)
	{
		m_skyRotate = 0.0f;
	}

	if (kb->pressed.Q || timer <= 0.0f)
	{
		ChangeScene<ResultScene>();
	}

	m_player->Update(elapsedTime, m_enemy->GetPos());
	m_enemy->Update(elapsedTime, m_player->GetPlayerPosition());
	m_camera.SetPlayer(m_player->GetPlayerPosition(), m_player->GetPlayerRotate());

	timer -= elapsedTime;

	if (kb->pressed.D0) { cameraNum = 0; }
	if (kb->pressed.D1) { cameraNum = 1; }
	if (kb->pressed.D2) { cameraNum = 2; }
	
	m_camera.Update(elapsedTime, cameraNum);

	m_timeNumber->SetNumber(timer);
}

void PlayScene::Render()
{
	// 一時的なカメラ設定
	// (リリースモードのバグ修正完了したら消す)
#if defined(_DEBUG)
	// ビュー行列を設定
	if (cameraNum == 0) {
		m_view = m_debugCamera->GetCameraMatrix();
	}
	else if (cameraNum != 0) {
		m_view = SimpleMath::Matrix::CreateLookAt(
			m_camera.GetEyePosition(),
			m_camera.GetTargetPosition(),
			SimpleMath::Vector3::UnitY
		);
	}
#else
	m_view = m_debugCamera->GetCameraMatrix();
#endif


	auto debugFont = GetUserResources()->GetDebugFont();
	auto device = GetUserResources()->GetDeviceResources()->GetD3DDevice();
	auto context = GetUserResources()->GetDeviceResources()->GetD3DDeviceContext();
	auto states = GetUserResources()->GetCommonStates();

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

	m_player->Render(context, states, m_view, m_proj);
	m_enemy->Render(context, states, m_view, m_proj);

	m_spriteBatch->Begin();
	m_timeNumber->Render();
	m_spriteBatch->End();
	
	

#if defined(_DEBUG)
	std::wostringstream PlayerHP;
	PlayerHP << "PlayerHP =  " << m_player->GetHP();
	debugFont->AddString(PlayerHP.str().c_str(), SimpleMath::Vector2(0.0f, debugFont->GetFontHeight() * 2), DirectX::Colors::Black);
	std::wostringstream EnemyHP;
	EnemyHP << "EnemyHP =  " << m_enemy->GetHP();
	debugFont->AddString(EnemyHP.str().c_str(), SimpleMath::Vector2(0.0f, debugFont->GetFontHeight() * 3), DirectX::Colors::Black);

#else
#endif
}

void PlayScene::Finalize()
{
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

	m_enemy = std::make_unique<Enemy>();
	m_enemy->Inisialize(device);

	m_spriteBatch = std::make_unique<SpriteBatch>(context);

	DX::ThrowIfFailed(CreateDDSTextureFromFile(device, L"Resources/Textures/number.dds", nullptr, PlayScene::m_numberSRV.ReleaseAndGetAddressOf()));
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
