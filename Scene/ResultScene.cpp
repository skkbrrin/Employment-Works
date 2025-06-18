#include "pch.h"
#include "ResultScene.h"


void ResultScene::Initialize()
{
	CreateDeviceDependentResources();
	CreateWindowSizeDependentResources();
}

void ResultScene::Update(float elapsedTime)
{
}

void ResultScene::Render()
{
	auto debugFont = GetUserResources()->GetDebugFont();
	debugFont->AddString(L"ResultScene", DirectX::SimpleMath::Vector2(0.0f, debugFont->GetFontHeight()));
}

void ResultScene::Finalize()
{
}

void ResultScene::CreateDeviceDependentResources()
{
}

void ResultScene::CreateWindowSizeDependentResources()
{
}

void ResultScene::OnDeviceLost()
{
}
