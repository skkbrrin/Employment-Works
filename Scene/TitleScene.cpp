#include "pch.h"
#include "TitleScene.h"



void TitleScene::Initialize()
{
	CreateDeviceDependentResources();
	CreateWindowSizeDependentResources();

	//m_menu->Initialize(m_deviceresources.get(), width, heigth);
}

void TitleScene::Update(float elapsedTime)
{
	UNREFERENCED_PARAMETER(elapsedTime);

	auto kb = DirectX::Keyboard::Get().GetState();

}

void TitleScene::Render()
{
	auto debugFont = GetUserResources()->GetDebugFont();
	debugFont->AddString(L"TitleScene", DirectX::SimpleMath::Vector2(0.0f, debugFont->GetFontHeight()));
}

void TitleScene::Finalize()
{
}

void TitleScene::CreateDeviceDependentResources()
{
}

void TitleScene::CreateWindowSizeDependentResources()
{
}

void TitleScene::OnDeviceLost()
{
}
