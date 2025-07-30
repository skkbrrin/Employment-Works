#include "pch.h"
#include "TutorialScene.h"
#include "ResultScene.h"

using namespace DirectX;

void TutorialScene::Initialize()
{
	CreateDeviceDependentResources();
	CreateWindowSizeDependentResources();

	m_camera.SetPlayer(m_player->GetPlayerPosition(), m_player->GetPlayerRotate());
}

void TutorialScene::Update(float elapsedTime)
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

	/*if (kb.Home) gameC = true;
	if (kb.End) gameC = false;

	if (gameC) {
		if (kb.Space) act = true;
	}
	if (act) m_camera.Update(elapsedTime, 1);
	else m_camera.Update(elapsedTime, 2);*/
}

void TutorialScene::Render()
{
	// ビュー行列を設定
	m_view = m_debugCamera->GetCameraMatrix();

	auto debugFont = GetUserResources()->GetDebugFont();
	debugFont->AddString(L"TutorialScene", DirectX::SimpleMath::Vector2(0.0f, debugFont->GetFontHeight()));

	auto device = GetUserResources()->GetDeviceResources()->GetD3DDevice();
	auto context = GetUserResources()->GetDeviceResources()->GetD3DDeviceContext();
	auto states = GetUserResources()->GetCommonStates();

	if (gameC) {
	m_view = SimpleMath::Matrix::CreateLookAt(
		m_camera.GetEyePosition(),
		m_camera.GetTargetPosition(),
		SimpleMath::Vector3::UnitY
	);
	}

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

	DirectX::SimpleMath::Matrix world = DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3(5.0f, 1.0f, 5.0f));
	m_enemy->Draw(context, *states, world, m_view, m_proj);
}

void TutorialScene::Finalize()
{
}

void TutorialScene::CreateDeviceDependentResources()
{
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
	m_enemy = DirectX::Model::CreateFromSDKMESH(device, L"Resources/Models/Enemy.sdkmesh", *fx);

	m_player = std::make_unique<Player>();
	m_player->Initialize(device);
}

void TutorialScene::CreateWindowSizeDependentResources()
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

void TutorialScene::OnDeviceLost()
{
}
