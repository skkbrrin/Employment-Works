#include "pch.h"
#include "Enemy.h"
#include "EnemyIdleState.h"
#include "EnemyDeathState.h"
#include "Charactor/Items/ItemManager.h"

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

void Enemy::TakeDamege(int dt)
{
	if (!m_isAlive)return;
	m_hp -= dt;

	if (m_hp <= 0)
	{
		Die();
		return;
	}

	Damaging();
}

void Enemy::Damaging()
{
}

void Enemy::Die()
{
	if (!m_isAlive) return;
	m_isAlive = false;

	// ここでドロップする（薪）
	if (m_itemManager)
	{
		// ランダムで 1～3 個
		int dropCount = (rand() % 3) + 1;

		for (int i = 0; i < dropCount; i++)
		{
			// 少し散らした位置でドロップ
			Matrix dropMat = Matrix::CreateTranslation(
				m_position +
				Vector3(
					((rand() % 100) - 50) * 0.05f,
					0.0f,
					((rand() % 100) - 50) * 0.05f
				)
			);

			m_itemManager->Spawn(Item::Type::Wood, dropMat);
		}
	}

	// 敵死亡ステートへ
	ChangeState(std::make_unique<EnemyDeathState>());
}



