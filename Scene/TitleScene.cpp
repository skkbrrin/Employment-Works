#include "pch.h"
#include "TitleScene.h"



void TitleScene::Initialize()
{
	CreateDeviceDependentResources();
	CreateWindowSizeDependentResources();

	auto deviceResouces = GetUserResources()->GetDeviceResources();

	//m_menu->Initialize(deviceResouces, 1280, 720);
}

void TitleScene::Update(float elapsedTime)
{
	UNREFERENCED_PARAMETER(elapsedTime);

	auto kb = DirectX::Keyboard::Get().GetState();

	//m_menu->Update();
}

void TitleScene::Render()
{
	auto debugFont = GetUserResources()->GetDebugFont();
	debugFont->AddString(L"TitleScene", DirectX::SimpleMath::Vector2(0.0f, debugFont->GetFontHeight()));

	//m_menu->Render();
}

void TitleScene::Finalize()
{
}

void TitleScene::CreateDeviceDependentResources()
{
	m_menu = std::make_unique<kHorikawa::Menu>();
}

void TitleScene::CreateWindowSizeDependentResources()
{
}

void TitleScene::OnDeviceLost()
{
}
