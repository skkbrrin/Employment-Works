#include "pch.h"
#include "Player.h"
#include "Charactor/Enemy/Enemy.h"
#include "PlayerIdleState.h"
#include "ResourceManager/Resource.h"

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
void Player::Initialize(ID3D11Device* device, DX::DeviceResources* dr)
{
	// モデル読み込み --------------------------------------------------------------------------------

	Resource::Initialize(device);
	m_root = std::make_unique<TransformNode>(L"Root"); 
	
	// 胴体
	auto body = std::make_unique<TransformNode>(L"Body");
	auto bodyModel = Resource::Model().Load(L"Resources/Models/DogBody.sdkmesh");
	body->SetModel(bodyModel);
	m_parts["Body"] = body.get(); 
	
	// 頭 
	auto head = std::make_unique<TransformNode>(L"Head");
	auto headModel = Resource::Model().Load(L"Resources/Models/DogHead.sdkmesh");
	head->SetModel(headModel);
	head->m_position = { 0.0f, 0.0f, 0.0f };
	m_parts["Head"] = head.get(); 
	
	// 斧 
	auto axe = std::make_unique<TransformNode>(L"Axe");
	auto axeModel = Resource::Model().Load(L"Resources/Models/Axe.sdkmesh");
	axe->SetModel(axeModel);
	axe->m_position = { 0.0f, 0.0f, 0.0f }; m_parts["Axe"] = axe.get();
	head->AddChild(std::move(axe)); 
	
	// 前足 
	auto legFL = std::make_unique<TransformNode>(L"LegFrontL");
	auto legFLModel = Resource::Model().Load(L"Resources/Models/LegFL.sdkmesh");
	legFL->SetModel(legFLModel);
	legFL->m_position = { -0.0f, -0.0f, 0.0f }; 
	m_parts["LegFrontL"] = legFL.get();
	
	auto legFR = std::make_unique<TransformNode>(L"LegFrontR");
	auto legFRModel = Resource::Model().Load(L"Resources/Models/LegFR.sdkmesh");
	legFR->SetModel(legFRModel);
	legFR->m_position = { 0.0f, -0.0f, 0.0f };
	m_parts["LegFrontR"] = legFR.get(); 
	
	// 後足 
	auto legBL = std::make_unique<TransformNode>(L"LegBackL"); 
	auto legBLModel = Resource::Model().Load(L"Resources/Models/LegBL.sdkmesh");
	legBL->SetModel(legBLModel);
	legBL->m_position = { -0.0f, -0.0f, -0.0f }; 
	m_parts["LegBackL"] = legBL.get(); 
	
	auto legBR = std::make_unique<TransformNode>(L"LegBackR");
	auto legBRModel = Resource::Model().Load(L"Resources/Models/LegBR.sdkmesh");
	legBR->SetModel(legBRModel);
	legBR->m_position = { 0.0f, -0.0f, -0.0f }; 
	m_parts["LegBackR"] = legBR.get(); 
	
	// しっぽ
	auto tail = std::make_unique<TransformNode>(L"Tail");
	auto tailModel = Resource::Model().Load(L"Resources/Models/Tail.sdkmesh");
	tail->SetModel(tailModel);
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

	// パーティクル生成
	m_dashParticle = std::make_unique<DashParticle>();
	m_dashParticle->Create(dr);

	m_spinWindParticle = std::make_unique<WindParticle>();
	m_spinWindParticle->Create(dr);

	m_spinDustParticle = std::make_unique<DustParticle>();
	m_spinDustParticle->Create(dr);
	
	// 初期状態をIdleに 
	ChangeState(std::make_unique<PlayerIdleState>());

	m_hp = m_maxHP;
	woodCount = 0;
}

void Player::Update(float dt)
{
	if (m_state) m_state->Update(this, dt);

	// 画面端
	if (m_position.x > 100.0f)
	{
		m_position.x = 100.0f;
	}
	if (m_position.z > 100.0f)
	{
		m_position.z = 100.0f;
	}

	if (m_invincibleTimer > 0.0f)
	{
		m_invincibleTimer -= dt;
		if (m_invincibleTimer < 0.0f)
			m_invincibleTimer = 0.0f;
	}

	// ジャンプ更新
	if (m_isJumping)
	{
		float gravity = 9.8f;

		m_jumpVelocity -= gravity * dt;

		Vector3 pos = GetPosition();

		pos.y += m_jumpVelocity * dt;

		// 地面
		if (pos.y <= 0.0f)
		{
			pos.y = 0.0f;
			m_jumpVelocity = 0.0f;
			m_isJumping = false;
		}

		SetPosition(pos);
	}

	// ノックバック処理
	if (m_isNockBack)
	{
		m_NockBackVelocity.y -= 12.5f * dt;
		m_position += m_NockBackVelocity * dt;

		// 地面に着地したら止める
		const float groundY = 0.0f;

		if (m_position.y <= groundY)
		{
			m_position.y = groundY;
			m_NockBackVelocity.y = 0.0f;
			m_isNockBack = false;
		}

		// 横方向減衰
		m_NockBackVelocity.x *= 0.98f;
		m_NockBackVelocity.z *= 0.98f;
	}

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
	if (IsBlink())
		return;

	if (m_root)
		m_root->Render(context, states, view, proj);

	m_dashParticle->Render(view, proj);
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

// ジャンプ
void Player::Jump()
{
	if (m_isJumping)
		return;

	m_isJumping = true;

	m_jumpVelocity = 8.0f;
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

void Player::TakeDamage(float dmg, const Vector3& attackerPos)
{

	if (IsInvincible())
		return;

	m_hp -= dmg;

	// -------- 無敵時間開始 --------
	m_invincibleTimer = 1.0f;

	// -------- ノックバック方向 --------
	Vector3 playerPos = GetPosition();
	Vector3 dir = playerPos - attackerPos;

	if (dir.LengthSquared() > 0.0001f)
	{
		dir.Normalize();

		// 横方向
		float knockPowerXZ = 6.0f;
		// 上方向
		float knockPowerY = 3.5f;

		m_NockBackVelocity = dir * knockPowerXZ;
		m_NockBackVelocity.y = knockPowerY;

		m_isNockBack = true;
	}
}
