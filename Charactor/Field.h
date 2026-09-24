#pragma once
#include <vector>
#include "Enemy/Enemy.h"
#include "Player/Player.h"
#include "Tree/Tree.h"
#include <GeometricPrimitive.h>
#include "Items/ItemManager.h"
#include "WoodManager.h"

class Field
{
private:
	static int ENEMY_COUNT; // 敵の数
	static int TREE_COUNT; // 木の数
public:
	// 初期化
	void Initialize(ID3D11Device* device, ID3D11DeviceContext* context, DX::DeviceResources* dr);
	// 更新
	void Update(float elapsedTime);
	// 描画
	void Render(ID3D11DeviceContext* context,
		DirectX::CommonStates* states,
		DirectX::SimpleMath::Matrix view,
		DirectX::SimpleMath::Matrix proj);

	// アイテム出現
	void SpawnItem(const DirectX::SimpleMath::Matrix& world);

	
private:
	Player* m_player; // プレイヤー
	std::vector<std::unique_ptr<Enemy>> m_enemies; // 敵
	std::vector<std::unique_ptr<Tree>> m_tries; // 木
	std::unique_ptr<DirectX::Model> m_lake; // 湖

	// 当たり判定
	std::unique_ptr<DirectX::GeometricPrimitive> m_debugOBB; 
	std::unique_ptr<DirectX::GeometricPrimitive> m_debugSphere;

	// 当たった情報
	struct HitInfo
	{
		std::vector<bool> enemyPartHit; 
		std::vector<bool> playerPartHit;
		std::vector<bool> treePartHit;
	};

	// 当たり判定
	// 確認
	void CheckCollision();

	// 更新
	void UpdateHitBox();

	// 描画
	void DrawHitBox(ID3D11DeviceContext* context, DirectX::CommonStates* states,
		const DirectX::BoundingOrientedBox& obb,
		const DirectX::SimpleMath::Matrix& view,
		const DirectX::SimpleMath::Matrix& proj,
		DirectX::XMVECTOR color)
	{
		using namespace DirectX::SimpleMath;

		Matrix world =
			Matrix::CreateScale(obb.Extents * 2.0f) *
			Matrix::CreateFromQuaternion(obb.Orientation) *
			Matrix::CreateTranslation(obb.Center);

		m_debugOBB->Draw(world, view, proj, color, nullptr, false);
	}

	// アイテムマネージャー
	ItemManager m_itemManager;

	// 出口の出現ができるか
	bool m_lakeAppeared = false;

	// リザルトに変わるかどうか
	bool m_canChangeScene = false;
public:
	// プレイヤーの情報取得
	Player* GetPlayer() { return m_player; }

	// 湖が出現したかどうか
	bool GetLakeAppeared() { return m_lakeAppeared; }

	// クリア判定
	bool GetClear() { return m_canChangeScene; }
};
