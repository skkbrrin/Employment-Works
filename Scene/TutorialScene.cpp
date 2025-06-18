#include "pch.h"
#include "TutorialScene.h"


void TutorialScene::Initialize()
{
	CreateDeviceDependentResources();
	CreateWindowSizeDependentResources();
}

void TutorialScene::Update(float elapsedTime)
{
}

void TutorialScene::Render()
{
	auto debugFont = GetUserResources()->GetDebugFont();
	debugFont->AddString(L"TutorialScene", DirectX::SimpleMath::Vector2(0.0f, debugFont->GetFontHeight()));
}

void TutorialScene::Finalize()
{
}

void TutorialScene::CreateDeviceDependentResources()
{
}

void TutorialScene::CreateWindowSizeDependentResources()
{
}

void TutorialScene::OnDeviceLost()
{
}
