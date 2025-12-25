#include "pch.h"
#include "PlayerPowerAttackState.h"
#include "PlayerIdleState.h"
#include "Player.h"

using namespace DirectX::SimpleMath;



void PlayerPowerAttackState::Enter(Player* player)
{
    m_timer = 0.0f;
    m_phase = SpinPhase::Start;


    Vector3 forward = Vector3::Transform(
        Vector3::UnitZ,
        player->GetRotation()
    );
    m_startAngle = atan2f(forward.x, forward.z);
    m_angle = m_startAngle;
}

void PlayerPowerAttackState::Update(Player* player, float elapsedTime)
{
    m_timer += elapsedTime;

    switch (m_phase)
    {
    case SpinPhase::Start:
        UpdateStart(player, elapsedTime);
        break;

    case SpinPhase::Spin:
        UpdateSpin(player, elapsedTime);
        break;

    case SpinPhase::End:
        UpdateEnd(player, elapsedTime);
        break;
    }

    // 回転適用（Y軸回転）
    DirectX::SimpleMath::Quaternion m_rot = DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(DirectX::SimpleMath::Vector3::UnitY, m_angle);
    player->SetRotation(m_rot);
}

void PlayerPowerAttackState::UpdateStart(Player* player, float elapsedTime)
{
    float t = m_timer / START_TIME;

    m_anglarVelocity = Lerp(m_startAngle, 8.0f, t);
    m_angle -= m_anglarVelocity * elapsedTime;

    // カメラ

    // 切り替え
    if (m_timer >= START_TIME)
    {
        m_timer = 0.0f;
        m_phase = SpinPhase::Spin;
    }
}

void PlayerPowerAttackState::UpdateSpin(Player* player, float elapsedTime)
{
    float t = Clamp(m_timer / (SPIN_TIME - 0.6f), 0.0f, 1.0f);

    player->SetPowerAttacking(true);
    m_anglarVelocity = 60.0f;
    m_angle -= m_anglarVelocity * elapsedTime;

    float radius = Lerp(0.0f, m_maxRadius, t);
    player->SetPowerAttackRadius(radius);
    // カメラ

    // エフェクト

    if (m_timer >= SPIN_TIME)
    {
        m_timer = 0.0f;
        m_phase = SpinPhase::End;
        m_endStartAngle = m_angle;
    }
}

void PlayerPowerAttackState::UpdateEnd(Player* player, float elapsedTime)
{
    float t = Clamp(m_timer / END_TIME, 0.0f, 1.0f);

    // 固定した角度 → 開始角へ補間
    m_angle = Lerp(m_endStartAngle, m_startAngle, t);

    if (m_timer >= END_TIME)
    {
        // カメラ

        // 土埃

        m_angle = m_startAngle;
        player->SetRotation(
            Quaternion::CreateFromAxisAngle(Vector3::UnitY, m_startAngle)
        );
        player->ChangeState(std::make_unique<PlayerIdleState>());
    }
}

void PlayerPowerAttackState::Exit(Player* player)
{
    player->SetPowerAttacking(false);
    player->GetPart("Body")->m_rotation = Quaternion::Identity;
}
