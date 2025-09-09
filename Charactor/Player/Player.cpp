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

	m_fullHP = 100;
	m_HP = m_fullHP;
	m_attck = 10;
}

void Player::Update(float elapsedTime, Enemy* enemy)
{
	auto kb = Keyboard::Get().GetState();
	m_tracker.Update(kb);


    // 攻撃

    if (m_isAttacking)
    {
        m_attackTime += elapsedTime;
        float t = m_attackTime / m_attackDuration;
        if (t > 1.0f) t = 1.0f;

        // 出発点→目標点を補間
        m_position = m_attackStartPos * (1 - t) + m_attackTargetPos * t;

        // パラボラ軌道（ジャンプっぽく）
        float jumpHeight = 2.0f;
        m_position.y += sinf(t * XM_PI) * jumpHeight;

        // 攻撃終了
        if (t >= 1.0f)
        {
            m_isAttacking = false;
            if (m_attackTarget)
            {
                m_attackTarget->Damage(m_attck);
                m_attackTarget = nullptr;
            }
        }
        return;
    }


    //通常移動
    if (kb.A) m_rotate = m_rotate * SimpleMath::Quaternion::CreateFromAxisAngle(SimpleMath::Vector3::UnitY, XMConvertToRadians(0.5f));
    if (kb.D) m_rotate = m_rotate * SimpleMath::Quaternion::CreateFromAxisAngle(SimpleMath::Vector3::UnitY, XMConvertToRadians(-0.5f));

    if (kb.W) m_position += SimpleMath::Vector3::Transform(SimpleMath::Vector3(0.0f, 0.0f, 0.1f), m_rotate);
    if (m_tracker.pressed.S) m_position -= SimpleMath::Vector3::Transform(SimpleMath::Vector3(0.0f, 0.0f, 2.5f), m_rotate);
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

void Player::Attack(const std::vector<std::unique_ptr<Enemy>>& enemies)
{
    if (m_isAttacking) return;
    if (enemies.empty()) return;

    Enemy* nearestEnemy = nullptr; // 一番近い敵
    float nearestDist = FLT_MAX; // 敵までの距離

    // 一番近い敵を探す
    for (auto& ene : enemies)
    {
        if (!ene || ene->GetIsDie()) continue;

        float dist = (ene->GetPos() - m_position).Length();
        if (dist < nearestDist)
        {
            nearestDist = dist;
            nearestEnemy = ene.get();
        }
    }

    if (!nearestEnemy) return;

    // プレイヤーの正面ベクトル
    DirectX::SimpleMath::Vector3 forward =
        DirectX::SimpleMath::Vector3::Transform(
            DirectX::SimpleMath::Vector3::UnitZ, m_rotate);
    forward.Normalize();

    // 敵方向ベクトル
    DirectX::SimpleMath::Vector3 toEnemy =
        nearestEnemy->GetPos() - m_position;
    toEnemy.Normalize();

    // 視野角チェック（±45°）
    float dot = forward.Dot(toEnemy);
    float angleThreshold = cosf(XMConvertToRadians(45.0f));
    if (dot < angleThreshold) return; // 前方じゃなければ攻撃失敗

    // 攻撃開始
    m_isAttacking = true;
    m_attackTime = 0.0f;
    m_attackStartPos = m_position;

    m_attackTargetPos = nearestEnemy->GetPos();
    m_attackTargetPos.y = -0.5f;

    m_attackTarget = nearestEnemy;
}

