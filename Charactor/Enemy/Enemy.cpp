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
	m_speed = SimpleMath::Vector3(1.0f, 0.0f, 0.0f);
}

void Enemy::Update(float elapsedTime)
{
	m_rotate *= SimpleMath::Quaternion::CreateFromAxisAngle(SimpleMath::Vector3::UnitY, XMConvertToRadians(10.f));

	m_speed = m_speed * 1.01f;

	if (rigth)
	{
		m_position += SimpleMath::Vector3::Transform(m_speed * elapsedTime, m_rotate);
	}
	else
	{
		m_position -= SimpleMath::Vector3::Transform(m_speed * elapsedTime, m_rotate);
	}
	

	if (m_position.x <= -30.0f) { rigth = true; }
	if (m_position.x >= 30.0f) { rigth = false; }
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
