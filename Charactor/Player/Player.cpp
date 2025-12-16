#include "pch.h"
#include "Player.h"
#include "Charactor/Enemy/Enemy.h"
#include "PlayerIdleState.h"
using namespace DirectX;

using namespace DirectX::SimpleMath;

Player::Player()
	: woodCount{0}
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
	legFR->m_position = { 0.0f, -0.0f, 0.0f };
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

// 当たり判定
std::vector<HitBoxPart> Player::GetHitBoxes() const
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;

	std::vector<HitBoxPart> result;

	// パーツ用配列(名前、中心、範囲、武器かどうか、敵に当たったらダメージを受けるか)
	struct PartDef { const char* name; Vector3 center; Vector3 extents; bool isWeapon; bool isDamageable; };

	// パーツの登録
	PartDef parts[] = {
		{"Body",      {0.0f,1.5f,0.0f},  {0.5f, 0.5f,1.1f}, false, true},
		{"Head",      {0.0f,2.65f,1.4f},  {0.7f,0.7f,0.5f}, false, true},
		{"LegFrontL", {-0.35f,0.5f,0.9f},   {0.2f,0.45f,0.2f}, false, true},
		{"LegFrontR", {0.35f,0.5f,0.9f},    {0.2f,0.45f,0.2f}, false, true},
		{"LegBackL",  {-0.35f,0.5f,-0.9f},  {0.2f,0.45f,0.2f}, false, true},
		{"LegBackR",  {0.35f,0.5f,-0.9f},  {0.2f,0.45f,0.2f}, false, true},
		{"Axe",       {1.35f,2.1f,2.8f},     {0.8f,0.05f,0.8f}, true, false}
	};

	// パーツ分当たり判定ボックスを作る
	for (auto& def : parts)
	{
		HitBoxPart hb;
		hb.name = def.name;
		hb.isWeapon = def.isWeapon;
		hb.isDamageable = def.isDamageable;

		auto it = m_parts.find(def.name);
		if (it != m_parts.end())
		{
			TransformNode* part = it->second;

			// OBBにする
			hb.obb.Center = def.center;
			hb.obb.Extents = def.extents;
			hb.obb.Orientation = Quaternion::Identity;

			// ワールド行列を反映
			hb.obb.Transform(hb.obb, part->GetWorldMatrix());
		}

		result.push_back(hb);
	}

	return result;
}

