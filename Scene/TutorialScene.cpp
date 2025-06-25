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
	auto device = GetUserResources()->GetDeviceResources()->GetD3DDevice();
	auto context = GetUserResources()->GetDeviceResources()->GetD3DDeviceContext();
	auto state = GetUserResources()->GetCommonStates();

	std::unique_ptr<DirectX::EffectFactory> fx = std::make_unique<DirectX::EffectFactory>(device);
	fx->SetDirectory(L"Resources/Materials");

	//m_player = DirectX::Model::CreateFromSDKMESH(device, L"Resources/Models/Player.sdkmesh", *fx);
	//m_enemy = DirectX::Model::CreateFromSDKMESH(device, L"Resources/Models/Enemy_1.sdkmesh", *fx);


}

void TutorialScene::CreateWindowSizeDependentResources()
{
}

void TutorialScene::OnDeviceLost()
{
}
