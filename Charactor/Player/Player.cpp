#include "pch.h"
#include "Player.h"
#include "PlayerIdleState.h"
using namespace DirectX;

using namespace DirectX::SimpleMath;

Player::Player()
{
}

Player::~Player()
{
}

void Player::Initialize(ID3D11Device* device)
{
	std::unique_ptr<EffectFactory> fx = std::make_unique<EffectFactory>(device); 
	fx->SetDirectory(L"Resources/Models"); 
	m_root = std::make_unique<TransformNode>(L"Root"); 
	
	// 胴体
	auto body = std::make_unique<TransformNode>(L"Body");
	body->SetModel(Model::CreateFromSDKMESH(device, L"Resources/Models/DogBody.sdkmesh", *fx));
	m_parts["Body"] = body.get(); 
	
	// 頭 
	auto head = std::make_unique<TransformNode>(L"Head");
	head->SetModel(Model::CreateFromSDKMESH(device, L"Resources/Models/DogHead.sdkmesh", *fx));
	head->m_position = { 0.0f, 0.0f, 0.0f };
	m_parts["Head"] = head.get(); 
	
	// 斧 
	auto axe = std::make_unique<TransformNode>(L"Axe");
	axe->SetModel(Model::CreateFromSDKMESH(device, L"Resources/Models/Axe.sdkmesh", *fx)); 
	axe->m_position = { 0.0f, 0.0f, 0.0f }; m_parts["Axe"] = axe.get();
	head->AddChild(std::move(axe)); 
	
	// 前足 
	auto legFL = std::make_unique<TransformNode>(L"LegFrontL");
	legFL->SetModel(Model::CreateFromSDKMESH(device, L"Resources/Models/LegFL.sdkmesh", *fx)); 
	legFL->m_position = { -0.0f, -0.0f, 0.0f }; 
	m_parts["LegFrontL"] = legFL.get();
	
	auto legFR = std::make_unique<TransformNode>(L"LegFrontR");
	legFR->SetModel(Model::CreateFromSDKMESH(device, L"Resources/Models/LegFR.sdkmesh", *fx)); 
	legFR->m_position = { 0.04f, -0.0f, 0.0f };
	m_parts["LegFrontR"] = legFR.get(); 
	
	// 後足 
	auto legBL = std::make_unique<TransformNode>(L"LegBackL"); 
	legBL->SetModel(Model::CreateFromSDKMESH(device, L"Resources/Models/LegBL.sdkmesh", *fx)); 
	legBL->m_position = { -0.0f, -0.0f, -0.0f }; 
	m_parts["LegBackL"] = legBL.get(); 
	
	auto legBR = std::make_unique<TransformNode>(L"LegBackR");
	legBR->SetModel(Model::CreateFromSDKMESH(device, L"Resources/Models/LegBR.sdkmesh", *fx));
	legBR->m_position = { 0.0f, -0.0f, -0.0f }; 
	m_parts["LegBackR"] = legBR.get(); 
	
	// しっぽ
	auto tail = std::make_unique<TransformNode>(L"Tail");
	tail->SetModel(Model::CreateFromSDKMESH(device, L"Resources/Models/Tail.sdkmesh", *fx)); 
	tail->m_position = { 0.0f, -0.0f, -0.0f }; m_parts["Tail"] = tail.get(); 
	
	// 階層構築 
	body->AddChild(std::move(head));
	body->AddChild(std::move(legFL));
	body->AddChild(std::move(legFR)); 
	body->AddChild(std::move(legBL)); 
	body->AddChild(std::move(legBR)); 
	body->AddChild(std::move(tail));
	m_root->AddChild(std::move(body));
	
	// 初期状態をIdleに 
	ChangeState(std::make_unique<PlayerIdleState>());
}

void Player::Update(float dt)
{
	// 状態更新
	if (m_state)
		m_state->Update(this, dt);

	// TransformNode の root をプレイヤー座標に合わせる
	if (m_root)
	{
		m_root->m_position = m_position;
		m_root->m_rotation = m_rotation;
	}

	// 子ノードのワールド行列更新
	if (m_root)
		m_root->UpdateWorldMatrix();
}

void Player::RenderP(ID3D11DeviceContext* context, DirectX::CommonStates* states, Matrix view, Matrix proj)
{
	if (m_root)
		m_root->Render(context, states, view, proj);
}
	

void Player::ChangeState(std::unique_ptr<PlayerState> newState)
{
	if (m_state) m_state->Exit(this);

	m_state = std::move(newState);

	if (m_state) m_state->Enter(this);
}

void Player::MoveForward(float dist)
{
	Vector3 forward = Vector3::Transform(Vector3::UnitZ, m_rotation);
	m_position += forward * dist;
}

void Player::RotateY(float deg)
{
	m_rotation = m_rotation *
		Quaternion::CreateFromAxisAngle(Vector3::UnitY, XMConvertToRadians(deg));
}

DirectX::BoundingBox Player::GetWeaponBox() const
{
	using namespace DirectX;

	TransformNode* weapon = GetPart("Axe");
	if (!weapon)
		return BoundingBox();

	// ローカル AABB
	BoundingBox localBox;
	BoundingBox::CreateFromPoints(
		localBox,
		SimpleMath::Vector3(-0.1f, -0.3f, -0.1f),
		SimpleMath::Vector3(0.1f, 0.3f, 0.1f)
	);

	// ワールド行列に変換
	BoundingBox worldBox;
	localBox.Transform(worldBox, weapon->GetWorldMatrix());
	return worldBox;
}
