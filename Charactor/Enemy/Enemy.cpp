#include "pch.h"
#include "Enemy.h"
#include "EnemyIdleState.h"
using namespace DirectX;

using namespace DirectX::SimpleMath;

Enemy::Enemy()
{
}

Enemy::~Enemy()
{
}

// 初期更新
void Enemy::Initialize(ID3D11Device* device)
{
	// モデル読み込み --------------------------------------------------------------------------------

	std::unique_ptr<EffectFactory> fx = std::make_unique<EffectFactory>(device); 
	fx->SetDirectory(L"Resources/Models"); 
	m_root = std::make_unique<TransformNode>(L"Root"); 
	
	// 胴体
	auto body = std::make_unique<TransformNode>(L"Body");
	body->SetModel(Model::CreateFromSDKMESH(device, L"Resources/Models/EnemyBody.sdkmesh", *fx));
	m_parts["Body"] = body.get(); 
	
	// 左手
	auto legL = std::make_unique<TransformNode>(L"LegL");
	legL->SetModel(Model::CreateFromSDKMESH(device, L"Resources/Models/EnemyLegL.sdkmesh", *fx)); 
	legL->m_position = { 0.0f, 0.0f, 0.0f }; 
	m_parts["LegL"] = legL.get();
	
	// 右手
	auto legR = std::make_unique<TransformNode>(L"LegR");
	legR->SetModel(Model::CreateFromSDKMESH(device, L"Resources/Models/EnemyLegR.sdkmesh", *fx)); 
	legR->m_position = { 0.0f, 0.0f, 0.0f };
	m_parts["LegR"] = legR.get(); 
	
	
	// ---------------------------------------------------------------------------------------------
	
	// 階層構築 
	body->AddChild(std::move(legL));
	body->AddChild(std::move(legR)); 
	m_root->AddChild(std::move(body));
	
	// 初期状態 
	ChangeState(std::make_unique<EnemyIdleState>());
}

void Enemy::Update(float dt)
{
	// 状態更新
	if (m_state)
		m_state->Update(this, dt);

	// ポジション合わせ
	if (m_root)
	{
		m_root->m_position = m_position;
		m_root->m_rotation = m_rotation;
	}

	// ワールド行列更新
	if (m_root)
		m_root->UpdateWorldMatrix();
}

// 描画
void Enemy::RenderE(ID3D11DeviceContext* context, DirectX::CommonStates* states, Matrix view, Matrix proj)
{
	if (m_root)
		m_root->Render(context, states, view, proj);
}
	
// 状態変更
void Enemy::ChangeState(std::unique_ptr<EnemyState> newState)
{
	if (m_state) m_state->Exit(this);

	m_state = std::move(newState);

	if (m_state) m_state->Enter(this);
}

// 移動
void Enemy::MoveForward(float dist)
{
	Vector3 forward = Vector3::Transform(Vector3::UnitZ, m_rotation);
	m_position += forward * dist;
}

// 回転
void Enemy::RotateY(float deg)
{
	m_rotation = m_rotation *
		Quaternion::CreateFromAxisAngle(Vector3::UnitY, XMConvertToRadians(deg));
}

// ローカルの当たり判定
std::vector<BoundingOrientedBox> Enemy::GetLocalHitBoxes() const
{
	std::vector<BoundingOrientedBox> boxes;

	BoundingOrientedBox body;
	body.Center = { 0, 0.9f, 0 };
	body.Extents = { 0.3f, 0.9f, 0.3f };
	body.Orientation = Quaternion::Identity;

	boxes.push_back(body);
	return boxes;
}

// ワールド座標の当たり判定
std::vector<BoundingOrientedBox> Enemy::GetWorldHitBoxes() const
{
	std::vector<BoundingOrientedBox> result;
	auto locals = GetLocalHitBoxes();

	auto it = m_parts.find("Body");
	if (it == m_parts.end())
		return result;

	TransformNode* body = it->second;
	auto world = body->GetWorldMatrix();

	for (auto& b : locals)
	{
		BoundingOrientedBox wb;
		b.Transform(wb, world);
		result.push_back(wb);
	}

	return result;
}
