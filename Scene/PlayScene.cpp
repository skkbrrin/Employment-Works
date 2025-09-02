#include "pch.h"
#include "PlayScene.h"
#include "ResultScene.h"

using namespace DirectX;

void PlayScene::Initialize()
{
	CreateDeviceDependentResources();
	CreateWindowSizeDependentResources();

	m_camera.SetPlayer(m_player->GetPlayerPosition(), m_player->GetPlayerRotate());
}

void PlayScene::Update(float elapsedTime)
{
	auto kb = Keyboard::Get().GetState();

	m_debugCamera->Update();


	float rotateSpeed = 1.0f;
	m_skyRotate += rotateSpeed * elapsedTime;

	if (m_skyRotate > 360.0f)
	{
		m_skyRotate = 0.0f;
	}

	if (kb.Q)
	{
		ChangeScene<ResultScene>();
	}

	m_player->Update(elapsedTime);
	m_enemy->Update(elapsedTime, m_player->GetPlayerPosition());

	timer += elapsedTime;
	
	//m_camera.Update(elapsedTime, 1);
}

void PlayScene::Render()
{
	// ビュー行列を設定
	m_view = m_debugCamera->GetCameraMatrix();

	/*m_view = SimpleMath::Matrix::CreateLookAt(
		m_camera.GetEyePosition(),
		m_camera.GetTargetPosition(),
		SimpleMath::Vector3::UnitY
	);*/

	auto debugFont = GetUserResources()->GetDebugFont();
	debugFont->AddString(L"PlayScene", DirectX::SimpleMath::Vector2(0.0f, debugFont->GetFontHeight()));
	std::wostringstream oss;
	oss << "Timer = " << timer;
	debugFont->AddString(oss.str().c_str(), SimpleMath::Vector2(0.0f, debugFont->GetFontHeight() * 2));

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
	//im = im * DirectX::SimpleMath::Matrix::CreateFromAxisAngle(DirectX::SimpleMath::Vector3::UnitY, DirectX::XMConvertToRadians(360.0f));

	//context->OMSetDepthStencilState(states->DepthNone(), 0);
	//context->RSSetState(states->CullNone());

	//m_skyModel->UpdateEffects([](IEffect* effect)
	//	{
	//		auto lights = dynamic_cast<IEffectLights*>(effect);
	//		if (lights)
	//		{
	//			lights->SetLightEnabled(0, true);

	//			DirectX::SimpleMath::Vector3 dir(0.0f, 0.0f, 0.0f);  // 逆向きに
	//			dir.Normalize();
	//			lights->SetLightDirection(0, DirectX::XMVectorSet(dir.x, dir.y, dir.z, 0.0f));

	//			lights->SetLightDiffuseColor(0, DirectX::Colors::White);
	//			lights->SetLightSpecularColor(0, DirectX::Colors::White);
	//			lights->SetAmbientLightColor(DirectX::Colors::WhiteSmoke);
	//		}
	//	});

	m_skyModel->Draw(context, *states, im * SimpleMath::Matrix::CreateScale(9000.0f), m_view, m_proj);
	// 天球-------------------------------------------------------------------------------------------------

	m_player->Render(context, states, m_view, m_proj);
	m_enemy->Render(context, states, m_view, m_proj);

	
	
	

#if defined(_DEBUG)
	std::wostringstream enemy;
	enemy << "EnemyRigth =  " << m_enemy->Getrigth() << "\n" << "EnemyPos =  " << m_enemy->GetPos();
	debugFont->AddString(enemy.str().c_str(), SimpleMath::Vector2(1000.0f, debugFont->GetFontHeight() * 2));

	std::wostringstream EnemyD;
	EnemyD << "Enemy<->Player =  " << m_enemy->GetDistance();
	debugFont->AddString(EnemyD.str().c_str(), SimpleMath::Vector2(1000.0f, debugFont->GetFontHeight() * 4));
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
