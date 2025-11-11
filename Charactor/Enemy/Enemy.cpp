#include "pch.h"
#include "Scene/PlayScene.h"
#include "Charactor/Enemy/Enemy.h"
#include "Charactor/Player/Player.h"

using namespace DirectX;

Enemy::Enemy()
{
}

Enemy::~Enemy()
{
}

void Enemy::Inisialize(ID3D11Device* device, ID3D11DeviceContext* context)
{
	std::unique_ptr<DirectX::EffectFactory> fx = std::make_unique<DirectX::EffectFactory>(device);
	fx->SetDirectory(L"Resources/Models");
	m_enemyModel = DirectX::Model::CreateFromSDKMESH(device, L"Resources/Models/Player.sdkmesh", *fx);

	m_position = GetPos();
	m_speed = 1.0f;

	m_HP = 10;
	m_attack = 10;

	m_collition = Ito::ModelCollisionFactory::CreateCollision
	(Ito::ModelCollision::CollisionType::OBB, m_enemyModel.get());

	m_displayCol = std::make_unique<Ito::DisplayCollision>(device, context);
}

void Enemy::Update(float elapsedTime, Player* player)
{
	// プレイヤーとの距離
	m_distance = (player->GetPlayerPosition() - m_position).LengthSquared();

	// ステート切り替え
	if (m_HP <= 0) { m_isDie = true; }
	else if (m_distance <= m_detectionRange * m_detectionRange) { m_state = State::Attack; }
	else { m_state = State::Chase; }

	// 状態に応じた処理
	switch (m_state)
	{
	case State::Chase:
		Chase(elapsedTime, player);
		break;
	case State::Attack:
		Attack(player, elapsedTime); // 移動はなし、攻撃アニメだけ
		break;
	case State::Roll:
		Roll(elapsedTime);
		break;
	}

	attackCooldown -= elapsedTime;

	m_collition->UpdateBoundingInfo(m_position, m_rotate);
	m_collition->AddDisplayCollision(m_displayCol.get());
}

void Enemy::Render(ID3D11DeviceContext* context, DirectX::CommonStates* states, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	SimpleMath::Matrix world;
	world = SimpleMath::Matrix::CreateFromQuaternion(m_rotate) * SimpleMath::Matrix::CreateTranslation(m_position);

	m_enemyModel->Draw(context, *states, world, view, proj);

	m_displayCol->DrawCollision(context, states, view, proj, Colors::Red);
}

void Enemy::Finalize()
{
}

void Enemy::Attack(Player* player, float elapsedTime)
{
	// 攻撃
	if (attackCooldown <= 0.0f)
	{
		player->Damage(m_attack);
		attackCooldown = 1.0f;
	}
}

void Enemy::Roll(float elapsedTime)
{
	// その場で回転
	m_rotate *= SimpleMath::Quaternion::CreateFromAxisAngle(
		SimpleMath::Vector3::UnitY,
		XMConvertToRadians(90.0f) * elapsedTime);
}

void Enemy::Chase(float elapsedTime, Player* player)
{
	SimpleMath::Vector3 toPlayer = player->GetPlayerPosition() - m_position;
	toPlayer.y = 0.0f; // 高さは固定

	if (toPlayer.LengthSquared() > 0.0001f)
		toPlayer.Normalize();

	// モデル前方向を +Z と仮定して角度計算
	float angle = atan2f(toPlayer.x, toPlayer.z);
	m_rotate = SimpleMath::Quaternion::CreateFromAxisAngle(SimpleMath::Vector3::UnitY, angle);

	// 移動
	m_position += toPlayer * m_speed * elapsedTime;
}

