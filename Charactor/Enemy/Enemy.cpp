#include "pch.h"
#include "Scene/PlayScene.h"
#include "Charactor/Enemy/Enemy.h"

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

	m_position = SimpleMath::Vector3(0.0f, 1.0f, 0.0f);
	m_speed = 3.0f;
}

void Enemy::Update(float elapsedTime, const DirectX::SimpleMath::Vector3 playerPos)
{
	// 方向ベクトル計算
	SimpleMath::Vector3 toPlayer = playerPos - m_position;

	if (toPlayer.LengthSquared() > 0.0001f)
	{
		toPlayer.Normalize();
	}

	// 移動
	m_position += toPlayer * m_speed * elapsedTime;

	// 向き
	float angle = atan2f(toPlayer.x, toPlayer.z);
	m_rotate = SimpleMath::Quaternion::CreateFromAxisAngle(SimpleMath::Vector3::UnitY, angle);
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
