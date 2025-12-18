#include "pch.h"
#include "Enemy.h"
#include "EnemyIdleState.h"
#include "EnemyDeathState.h"
#include "EnemyAttackState.h"
#include "Charactor/Items/ItemManager.h"
#include "Charactor/Player/Player.h"
#include "ResourceManager/Resource.h"

using namespace DirectX;

using namespace DirectX::SimpleMath;

bool Enemy::s_anyEnemyDied = false;

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

	Resource::Initialize(device);

	m_root = std::make_unique<TransformNode>(L"Root"); 
	
	// 胴体
	auto body = std::make_unique<TransformNode>(L"Body");
	auto bodyModel = Resource::Model().Load( L"Resources/Models/EnemyBody.sdkmesh");
	body->SetModel(bodyModel);
	m_parts["Body"] = body.get(); 
	
	// 左手
	auto legL = std::make_unique<TransformNode>(L"LegL");
	auto legLModel = Resource::Model().Load( L"Resources/Models/EnemyLegL.sdkmesh");
	legL->SetModel(legLModel);
	legL->m_position = { 0.0f, 0.0f, 0.0f }; 
	m_parts["LegL"] = legL.get();
	
	// 右手
	auto legR = std::make_unique<TransformNode>(L"LegR");
	auto legRModel = Resource::Model().Load(L"Resources/Models/EnemyLegR.sdkmesh");
	legR->SetModel(legRModel);
	legR->m_position = { 0.0f, 0.0f, 0.0f };
	m_parts["LegR"] = legR.get(); 
	
	
	// ---------------------------------------------------------------------------------------------
	
	// 階層構築 
	body->AddChild(std::move(legL));
	body->AddChild(std::move(legR)); 
	m_root->AddChild(std::move(body));
	
	// 初期状態 
	ChangeState(std::make_unique<EnemyIdleState>());
	m_attackState = false;
}

void Enemy::Update(float dt)
{
	// 状態更新
	if (s_anyEnemyDied && !m_attackState)
	{
		m_attackState = true;
		Damaging();
	}

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
	m_rotation = Quaternion::CreateFromAxisAngle(Vector3::UnitY, deg);
}


std::vector<HitBoxPart> Enemy::GetHitBoxes() const
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;

	std::vector<HitBoxPart> result;

	// パーツ用配列(名前、中心、範囲、武器かどうか、プレイヤーに当たったらダメージを受けるか)
	struct PartDef { const char* name; Vector3 center; DirectX::SimpleMath::Quaternion rotation; Vector3 extents; bool isWeapon; bool isDamageable; };

	// パーツの登録
	PartDef parts[] = {
		{"Body", {0,4.0f,0},  DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(0.0f, 0.0f, 0.0f),  {0.7f,4.0f,0.7f}, false, true},
		{"LegL", {-2.1f,4.1f,0}, DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(0.0f, 0.0f, XMConvertToRadians(65.0f)) ,  {0.25f,1.7f,0.3f}, true, true},
		{"LegR", {1.5f,4.15f,0}, DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(0.0f, 0.0f, XMConvertToRadians(-62.0f)) ,  {0.25f,2.1f,0.25f}, true, true},
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
			hb.obb.Orientation = def.rotation;

			// ワールド行列を反映
			hb.obb.Transform(hb.obb, part->GetWorldMatrix());
		}

		result.push_back(hb);
	}

	return result;
}

void Enemy::TakeDamage(int dmg)
{
	if (!m_isAlive) return;
	m_hp -= dmg;

	if (m_hp <= 0)
	{
		Die();

		if (!s_anyEnemyDied)
		{
			s_anyEnemyDied = true;
		}

		return;
	}
}

void Enemy::Damaging()
{
	if (!m_isAlive) return;   // ← 保険
	ChangeState(std::make_unique<EnemyAttackState>());
}

void Enemy::Die()
{
	if (!m_isAlive) return;
	m_isAlive = false;

	// ここでドロップする（薪）
	if (m_itemManager)
	{
		int dropCount = (rand() % 3) + 12;

		for (int i = 0; i < dropCount; i++)
		{
			// 初期位置（敵の位置）
			Vector3 startPos = m_position;

			// 初速ランダム（水平）
			float vx = ((rand() % 100) - 50) * 0.05f; // -2.5 ～ +2.5
			float vz = ((rand() % 100) - 50) * 0.05f;

			// 上方向にポーンと飛ばす
			float vy = (rand() % 30) * 0.1f + 2.0f;   // 2.0～5.0ぐらい

			Vector3 velocity(vx, vy, vz);

			// ItemManagerに初速も渡す
			m_itemManager->SpawnWithVelocity(Item::Type::Wood, startPos, velocity);
		}
	}

	// 敵死亡ステートへ
	ChangeState(std::make_unique<EnemyDeathState>());
}

Vector3& Enemy::GetPlayerPosition()
{
	return m_player->GetPosition();
}
