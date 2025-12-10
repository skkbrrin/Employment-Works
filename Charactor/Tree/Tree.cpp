#include "pch.h"
#include "Tree.h"
#include "TreeDeathState.h"
#include "Charactor/Items/ItemManager.h"
#include "Charactor/Player/Player.h"

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

	std::unique_ptr<EffectFactory> fx = std::make_unique<EffectFactory>(device); 
	fx->SetDirectory(L"Resources/Models"); 
	m_root = std::make_unique<TransformNode>(L"Root"); 
	
	// 胴体
	auto body = std::make_unique<TransformNode>(L"Body");
	body->SetModel(Model::CreateFromSDKMESH(device, L"Resources/Models/TreeBody.sdkmesh", *fx));
	m_parts["Body"] = body.get(); 
	
	// 左手
	auto legL = std::make_unique<TransformNode>(L"LegL");
	legL->SetModel(Model::CreateFromSDKMESH(device, L"Resources/Models/TreeLegL.sdkmesh", *fx)); 
	legL->m_position = { 0.0f, 0.0f, 0.0f }; 
	m_parts["LegL"] = legL.get();
	
	// 右手
	auto legR = std::make_unique<TransformNode>(L"LegR");
	legR->SetModel(Model::CreateFromSDKMESH(device, L"Resources/Models/TreeLegR.sdkmesh", *fx)); 
	legR->m_position = { 0.0f, 0.0f, 0.0f };
	m_parts["LegR"] = legR.get(); 
	
	
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

void Tree::TakeDamage(int dt)
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

	ChangeState(std::make_unique<TreeDeathState>());
}

Vector3& Tree::GetPlayerPosition()
{
	return m_player->GetPosition();
}
