#include "pch.h"
#include "PlayerDashAttackState.h"
#include "PlayerIdleState.h"
#include "Player.h"

using namespace DirectX::SimpleMath;

void PlayerDashAttackState::Enter(Player* player)
{
    m_timer = 0.0f;

    player->SetDashAttacking(true);

    // 現在向いている方向をそのままダッシュ方向にする
    m_dashDir = player->GetForward();
    m_dashDir.Normalize();

    player->GetDashParticle()->Start();
}

void PlayerDashAttackState::Update(Player* player, float elapsedTime)
{
    m_timer += elapsedTime;

    player->SetPowerAttackRadius(8.0f);

    constexpr float DashSpeed = 300.0f;
    player->GetPosition() += m_dashDir * DashSpeed * elapsedTime;

    float swing = sinf(m_timer * 10.0f);
    auto head = player->GetPart("Head");

    head->m_rotation =
        Quaternion::CreateFromAxisAngle(Vector3::UnitY, swing * 0.8f);

    if (m_timer > 0.2f)
    {
        player->GetDashParticle()->Stop();
        player->ChangeState(std::make_unique<PlayerIdleState>());
        return;
    }
}

void PlayerDashAttackState::Exit(Player* player)
{
    // リセット
    player->GetPart("Head")->m_rotation = Quaternion::Identity;
    player->GetPart("Axe")->m_rotation = Quaternion::Identity;
    player->SetDashAttacking(false);
}
