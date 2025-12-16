#pragma once
#include <vector>
#include "Enemy/Enemy.h"
#include "Player/Player.h"
#include "Tree/Tree.h"
#include <GeometricPrimitive.h>
#include "Items/ItemManager.h"

class Field
{
public:
	void Initialize(ID3D11Device* device, ID3D11DeviceContext* context);
	void Update(float elapsedTime);
	void Render(ID3D11DeviceContext* context,
		DirectX::CommonStates* states,
		DirectX::SimpleMath::Matrix view,
		DirectX::SimpleMath::Matrix proj);
	void SpawnItem(const DirectX::SimpleMath::Matrix& world);

	
private:
	Player* m_player;
	std::vector<std::unique_ptr<Enemy>> m_enemies;
	std::vector<std::unique_ptr<Tree>> m_tries;

	std::unique_ptr<DirectX::GeometricPrimitive> m_debugOBB;

	struct HitInfo
	{
		std::vector<bool> enemyPartHit; 
		std::vector<bool> playerPartHit;
		std::vector<bool> treePartHit;
	};

	std::unordered_map<Enemy*, HitInfo> m_hitMap;

	void CheckCollision();

	void UpdateHitBox();

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

	ItemManager m_itemManager;

public:
	Player* GetPlayer() { return m_player; }

};
