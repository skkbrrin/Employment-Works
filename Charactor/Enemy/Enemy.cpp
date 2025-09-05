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

void Enemy::Inisialize(ID3D11Device* device)
{
	std::unique_ptr<DirectX::EffectFactory> fx = std::make_unique<DirectX::EffectFactory>(device);
	fx->SetDirectory(L"Resources/Models");
	m_enemyModel = DirectX::Model::CreateFromSDKMESH(device, L"Resources/Models/Enemy.sdkmesh", *fx);

	m_position = SimpleMath::Vector3(10.0f, 1.0f, 10.0f);
	m_speed = 3.0f;

	m_HP = 10;
	m_attack = 10;
}

void Enemy::Update(float elapsedTime, Player* player)
{
	// プレイヤーとの距離
	m_distance = (player->GetPlayerPosition() - m_position).LengthSquared();

	// ステート切り替え
	if (m_distance <= m_detectionRange * m_detectionRange) { m_state = State::Attack; }
	else { m_state = State::Chase; }

	if (m_state == State::Chase) { Chase(elapsedTime, player);
	}
	else if (m_state == State::Roll) { Roll(elapsedTime); }
	else if (m_state == State::Attack) { Attack(player); }

	attackCooldown -= elapsedTime;
}

void Enemy::Render(ID3D11DeviceContext* context, DirectX::CommonStates* states, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	SimpleMath::Matrix world;
	world = SimpleMath::Matrix::CreateFromQuaternion(m_rotate) * SimpleMath::Matrix::CreateTranslation(m_position);

	m_enemyModel->Draw(context, *states, world, view, proj);

}

void Enemy::Finalize()
{
}

void Enemy::Attack(Player* player)
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
	// 方向ベクトル計算
	SimpleMath::Vector3 toPlayer = player->GetPlayerPosition() - m_position;

	if (toPlayer.LengthSquared() > 0.0001f)
	{
		toPlayer.Normalize();
	}

	// 移動
	m_position += toPlayer * m_speed * elapsedTime;
	m_position.y = 1.0f;

	// 向き
	float angle = atan2f(toPlayer.x, toPlayer.z);
	m_rotate = SimpleMath::Quaternion::CreateFromAxisAngle(SimpleMath::Vector3::UnitY, angle);
}
