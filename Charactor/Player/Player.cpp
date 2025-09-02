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
	m_player = DirectX::Model::CreateFromSDKMESH(device, L"Resources/Models/Player.sdkmesh", *fx);
}

void Player::Update(float elapsedTime)
{
	auto kb = DirectX::Keyboard::Get().GetState();

	//Player
	m_position.y = -1.0f;
	if (kb.A) m_rotate = m_rotate * SimpleMath::Quaternion::CreateFromAxisAngle(SimpleMath::Vector3::UnitY, XMConvertToRadians(0.5f));
	if (kb.D) m_rotate = m_rotate * SimpleMath::Quaternion::CreateFromAxisAngle(SimpleMath::Vector3::UnitY, XMConvertToRadians(-0.5f));

	if (kb.W) m_position += SimpleMath::Vector3::Transform(SimpleMath::Vector3(0.0f, 0.0f, 0.1f), m_rotate);
	if (kb.S) m_position -= SimpleMath::Vector3::Transform(SimpleMath::Vector3(0.0f, 0.0f, 0.1f), m_rotate);

}

void Player::Render(ID3D11DeviceContext* context, DirectX::CommonStates* states, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	SimpleMath::Matrix p_world;
	p_world = SimpleMath::Matrix::CreateFromQuaternion(m_rotate) * SimpleMath::Matrix::CreateTranslation(m_position);

	m_player->Draw(context, *states, p_world, view, proj);

}

void Player::Finalize()
{
}

