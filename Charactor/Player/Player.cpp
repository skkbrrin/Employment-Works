#include "pch.h"
#include "Player.h"
#include "Charactor/Enemy/Enemy.h"
#include "PlayerIdleState.h"
using namespace DirectX;

using namespace DirectX::SimpleMath;

Player::Player()
{
}

Player::~Player()
{
}

// 初期更新
void Player::Initialize(ID3D11Device* device)
{
	// モデル読み込み --------------------------------------------------------------------------------

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

	// ---------------------------------------------------------------------------------------------
	
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
	if (m_state) m_state->Update(this, dt);

	if (m_root)
	{
		// CharacterBase のワールド行列を反映
		m_root->SetWorldParent(GetWorldMatrix());
		m_root->UpdateWorldMatrix();
	}
}

// 描画
void Player::RenderP(ID3D11DeviceContext* context, DirectX::CommonStates* states, Matrix view, Matrix proj)
{
	if (m_root)
		m_root->Render(context, states, view, proj);
}
	
// 状態変更
void Player::ChangeState(std::unique_ptr<PlayerState> newState)
{
	if (m_state) m_state->Exit(this);

	m_state = std::move(newState);

	if (m_state) m_state->Enter(this);
}

// 移動
void Player::MoveForward(float dist)
{
	Vector3 f = Vector3::Transform(Vector3::UnitZ, m_rotation);
	m_position += f * dist;
}

// 回転
void Player::RotateY(float deg)
{
	m_rotation = m_rotation *
		Quaternion::CreateFromAxisAngle(Vector3::UnitY,
			XMConvertToRadians(deg));
}

// ローカルの当たり判定
std::vector<BoundingOrientedBox> Player::GetLocalHitBoxes() const
{
	std::vector<BoundingOrientedBox> boxes;

	BoundingOrientedBox axe;
	axe.Center = { 0,0,0.4f };
	axe.Extents = { 0.15f, 0.3f, 0.6f };
	axe.Orientation = Quaternion::Identity;

	boxes.push_back(axe);
	return boxes;
}

// ワールド座標の当たり判定
std::vector<BoundingOrientedBox> Player::GetWorldHitBoxes() const
{
	auto locals = GetLocalHitBoxes();
	std::vector<BoundingOrientedBox> result;

	TransformNode* axe = GetPart("Axe");
	if (!axe) return result;

	for (auto& b : locals)
	{
		BoundingOrientedBox wb;
		b.Transform(wb, axe->GetWorldMatrix());
		result.push_back(wb);
	}
	return result;
}