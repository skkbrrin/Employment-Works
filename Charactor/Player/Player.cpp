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

        // ジャンプ
        if (t < 0.5f)
        {
            float tt = t / 0.5f; // 正規化

            m_position = m_attackStartPos;
            float jumpHeigth = 2.0f; // ジャンプの高さ
            m_position.y += sinf(tt * XM_PI) * jumpHeigth; // ジャンプ
        }
        
        // 斜め移動切り
        else
        {
            float tt = t / 0.5f; // 正規化

            // 開始位置
            DirectX::SimpleMath::Vector3 apex = m_attackStartPos;
            apex.y += 2.0f;

            // 直線移動
            m_position.x = apex.x * (1 - tt) + m_attackTargetPos.x * tt;
            m_position.y = apex.y * (1 - tt) + m_attackTargetPos.y * tt + 2.0f;
            m_position.z = apex.z * (1 - tt) + m_attackTargetPos.z * tt;
        }

        // 攻撃終了
        if (t >= 1.0f)
        {
            m_isAttacking = false;
            SetAttacking(false);
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

void Player::Attack(float elapsedTime, const std::vector<std::unique_ptr<Enemy>>& enemies)
{
    // 攻撃中は時間経過で動作
    if (m_isAttacking)
    {
        m_attackTime += elapsedTime;
        float t = m_attackTime / m_attackDuration;
        if (t > 1.0f) t = 1.0f;

        // 攻撃前半：ジャンプ
        if (t < 0.5f)
        {
            float tt = t / 0.5f;
            m_position = m_attackStartPos;
            float jumpHeight = 2.0f;
            m_position.y += sinf(tt * XM_PI) * jumpHeight;
        }
        // 攻撃後半：斜め移動
        else
        {
            float tt = (t - 0.5f) / 0.5f; // 正規化
            DirectX::SimpleMath::Vector3 apex = m_attackStartPos;
            apex.y += 2.0f;

            m_position.x = apex.x * (1 - tt) + m_attackTargetPos.x * tt;
            m_position.y = apex.y * (1 - tt) + m_attackTargetPos.y * tt;
            m_position.z = apex.z * (1 - tt) + m_attackTargetPos.z * tt;
        }

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

    // 攻撃開始判定
    if (enemies.empty()) return;

    Enemy* nearestEnemy = nullptr;
    float nearestDist = FLT_MAX;

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

    DirectX::SimpleMath::Vector3 forward =
        DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3::UnitZ, m_rotate);
    forward.Normalize();

    DirectX::SimpleMath::Vector3 toEnemy = nearestEnemy->GetPos() - m_position;
    toEnemy.Normalize();

    float dot = forward.Dot(toEnemy);
    float angleThreshold = cosf(XMConvertToRadians(45.0f));
    if (dot < angleThreshold) return;

    // 攻撃開始
    m_isAttacking = true;
    m_attackTime = 0.0f;
    m_attackStartPos = m_position;
    m_attackTargetPos = nearestEnemy->GetPos();
    m_attackTargetPos.y = -0.5f;
    m_attackTarget = nearestEnemy;
}


