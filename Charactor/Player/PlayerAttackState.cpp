#include "pch.h"
#include "PlayerAttackState.h"
#include "PlayerIdleState.h"
#include "PlayerPowerAttackState.h"
#include "Player.h"

using namespace DirectX::SimpleMath;

void PlayerAttackState::Enter(Player* player)
{
    m_timer = 0.0f;
}

void PlayerAttackState::Update(Player* player, float elapsedTime)
{
    m_kb = DirectX::Keyboard::Get().GetState();
    m_kbTracker.Update(m_kb);

    m_timer += elapsedTime;

    float swing = sinf(m_timer * 10.0f);
    auto head = player->GetPart("Head");
    auto axe = player->GetPart("Axe");

    // 攻撃アニメーション
    head->m_rotation = Quaternion::CreateFromAxisAngle(Vector3::UnitY, swing * 0.8f);

    // 攻撃連打してるかどうか
    if (m_spaceCount > 0)
    {
        m_typeTimer += elapsedTime;
    }

    if (m_typeTimer >= 0.5f)
    {
        m_spaceCount = 0;
        m_typeTimer = 0.0f;
    }

    if (m_kbTracker.pressed.Space)
    {
        m_spaceCount++;
        m_typeTimer = 0.0f;
    }

    // 範囲攻撃
    if (m_spaceCount >= 5)
    {
        player->ChangeState( std::make_unique<PlayerPowerAttackState>());
        return;
    }

    // 連打無し 攻撃終了
    if (m_timer > 0.65f)
    {
        player->ChangeState(std::make_unique<PlayerIdleState>());
        return;
    }
}

void PlayerAttackState::Exit(Player* player)
{
    // リセット
    player->GetPart("Head")->m_rotation = Quaternion::Identity;
    player->GetPart("Axe")->m_rotation = Quaternion::Identity;
}
