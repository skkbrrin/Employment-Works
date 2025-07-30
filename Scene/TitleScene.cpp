#include "pch.h"
#include "TitleScene.h"
#include "TutorialScene.h"


void TitleScene::Initialize()
{
	CreateDeviceDependentResources();
	CreateWindowSizeDependentResources();

	auto deviceResouces = GetUserResources()->GetDeviceResources();
	auto device = GetUserResources()->GetDeviceResources()->GetD3DDevice();

	//m_menu->Initialize(deviceResouces, 1280, 720);
	m_texBill->Create(deviceResouces, device);
}

void TitleScene::Update(float elapsedTime)
{
	UNREFERENCED_PARAMETER(elapsedTime);

	auto kb = DirectX::Keyboard::Get().GetState();

	if (kb.Space)
	{
		ChangeScene<TutorialScene>();
	}

	//m_menu->Update();
	m_texBill->CreateBillboard(m_debugCamera->GetEyePosition(), DirectX::SimpleMath::Vector3::Up);
}

void TitleScene::Render()
{
	// ビュー行列を設定
	m_view = m_debugCamera->GetCameraMatrix();

	auto debugFont = GetUserResources()->GetDebugFont();
	debugFont->AddString(L"TitleScene", DirectX::SimpleMath::Vector2(0.0f, debugFont->GetFontHeight()));

	//m_menu->Render();
	m_texBill->Render(m_view, m_proj);
}

void TitleScene::Finalize()
{
}

void TitleScene::CreateDeviceDependentResources()
{
	m_menu = std::make_unique<kHorikawa::Menu>();

	m_texBill = std::make_unique<TextureBillboard>();
}

void TitleScene::CreateWindowSizeDependentResources()
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

void TitleScene::OnDeviceLost()
{
}
