#include "pch.h"
#include "Tree.h"
#include "TreeDeathState.h"
#include "Charactor/Items/ItemManager.h"
#include "Charactor/Player/Player.h"
#include "ResourceManager/Resource.h"

using namespace DirectX;

using namespace DirectX::SimpleMath;

Tree::Tree()
{
}

Tree::~Tree()
{
}

// 初期更新
void Tree::Initialize(ID3D11Device* device)
{
	// モデル読み込み --------------------------------------------------------------------------------
	Resource::Initialize(device);

	m_root = std::make_unique<TransformNode>(L"Root"); 
	
	// 胴体
	auto body = std::make_unique<TransformNode>(L"Body");
	auto bodyModel = Resource::Model().Load(L"Resources/Models/TreeBody.sdkmesh");
	body->SetModel(bodyModel);
	m_parts["Body"] = body.get(); 
	
	// 左手
	auto legL = std::make_unique<TransformNode>(L"LegL");
	auto legLModel = Resource::Model().Load(L"Resources/Models/TreeLegL.sdkmesh");
	legL->SetModel(legLModel);
	legL->m_position = { 3.0f, -1.0f, 0.0f }; 
	m_parts["LegL"] = legL.get();
	
	// 右手
	auto legR = std::make_unique<TransformNode>(L"LegR");
	auto legRModel = Resource::Model().Load(L"Resources/Models/TreeLegR.sdkmesh");
	legR->SetModel(legRModel);
	legR->m_position = { 1.5f, 0.0f, 0.0f };
	m_parts["LegR"] = legR.get(); 
	
	body->m_scale = { 1.5, 2, 1.5 };
	legL->m_scale = { 1.5, 1, 1.5 };
	legR->m_scale = { 1.5, 1, 1.5 };
	
	// ---------------------------------------------------------------------------------------------
	
	// 階層構築 
	body->AddChild(std::move(legL));
	body->AddChild(std::move(legR)); 
	m_root->AddChild(std::move(body));
	
}

void Tree::Update(float dt)
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
void Tree::RenderT(ID3D11DeviceContext* context, DirectX::CommonStates* states, Matrix view, Matrix proj)
{
	if (m_root)
		m_root->Render(context, states, view, proj);
}
	
// 状態変更
void Tree::ChangeState(std::unique_ptr<TreeState> newState)
{
	if (m_state) m_state->Exit(this);

	m_state = std::move(newState);

	if (m_state) m_state->Enter(this);
}

std::vector<HitBoxPart> Tree::GetHitBoxes() const
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;

	std::vector<HitBoxPart> result;

	// パーツ用配列(名前、中心、回転角、 範囲、武器かどうか、プレイヤーに当たったらダメージを受けるか)
	struct PartDef { const char* name; Vector3 center; DirectX::SimpleMath::Quaternion rotation; Vector3 extents; bool isWeapon; bool isDamageable; };

	// パーツの登録
	PartDef parts[] = {
		{"Body", {-4.3f,4.0f,0},  DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(0.0f, 0.0f, 0.0f),  {0.7f,4.0f,0.7f}, false, true},
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

void Tree::TakeDamage(float dt)
{
	if (!m_isAlive) return;
	m_hp -= dt;

	if (m_hp <= 0)
	{
		Die();
		return;
	}

	Damaging();
}

void Tree::Damaging()
{
}

void Tree::Die()
{
	if (!m_isAlive) return;
	m_isAlive = false;

	m_shouldDropItem = true;   // ← フラグだけ

	ChangeState(std::make_unique<TreeDeathState>());
}

Vector3& Tree::GetPlayerPosition()
{
	return m_player->GetPosition();
}
