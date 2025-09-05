#include "pch.h"
#include "Charactor/Player/Player.h"
#include "Scene/PlayScene.h"

using namespace DirectX;

Player::Player()
{
}

Player::~Player()
{
}

void Player::Initialize( ID3D11Device* device )
{
	std::unique_ptr<DirectX::EffectFactory> fx = std::make_unique<DirectX::EffectFactory>(device);
	fx->SetDirectory(L"Resources/Models");
	m_player = DirectX::Model::CreateFromSDKMESH(device, L"Resources/Models/Dog.sdkmesh", *fx);

	m_rotate = SimpleMath::Quaternion::Identity;
	m_position = SimpleMath::Vector3(0.0f, -0.5f, 0.0f);

	m_HP = 100;
	m_attck = 10;
}

void Player::Update(float elapsedTime, const DirectX::SimpleMath::Vector3 enemyPos)
{
	auto kb = Keyboard::Get().GetState();
	m_tracker.Update(kb);

	//Player
	if (kb.A) m_rotate = m_rotate * SimpleMath::Quaternion::CreateFromAxisAngle(SimpleMath::Vector3::UnitY, XMConvertToRadians(0.5f));
	if (kb.D) m_rotate = m_rotate * SimpleMath::Quaternion::CreateFromAxisAngle(SimpleMath::Vector3::UnitY, XMConvertToRadians(-0.5f));

	if (kb.W) m_position += SimpleMath::Vector3::Transform(SimpleMath::Vector3(0.0f, 0.0f, 0.1f), m_rotate);
	if (m_tracker.pressed.S) m_position -= SimpleMath::Vector3::Transform(SimpleMath::Vector3(0.0f, 0.0f, 2.5f), m_rotate);

	if (m_tracker.pressed.Z)
	{
		m_position = SimpleMath::Vector3(enemyPos.x, -0.5f, enemyPos.z);
	}
}

void Player::Render(ID3D11DeviceContext* context, DirectX::CommonStates* states, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	SimpleMath::Matrix p_world;
	p_world = SimpleMath::Matrix::CreateRotationY(XMConvertToRadians(180.0f)) * SimpleMath::Matrix::CreateFromQuaternion(m_rotate) * SimpleMath::Matrix::CreateTranslation(m_position);

	m_player->Draw(context, *states, p_world, view, proj);

}

void Player::Finalize()
{
}

