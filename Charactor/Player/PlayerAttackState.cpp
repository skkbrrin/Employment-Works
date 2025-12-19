#include "pch.h"
#include "PlayerAttackState.h"
#include "PlayerIdleState.h"
#include "Player.h"

using namespace DirectX::SimpleMath;

void PlayerAttackState::Enter(Player* player)
{
    m_timer = 0.0f;
}

void PlayerAttackState::Update(Player* player, float elapsedTime)
{
    m_timer += elapsedTime;

    float swing = sinf(m_timer * 10.0f);
    auto head = player->GetPart("Head");
    auto axe = player->GetPart("Axe");

    // 攻撃アニメーション
    head->m_rotation = Quaternion::CreateFromAxisAngle(Vector3::UnitY, swing * 0.8f);

    // 攻撃終了
    if (m_timer > 0.65f)
    {
        player->ChangeState(std::make_unique<PlayerIdleState>());
    }
}

void PlayerAttackState::Exit(Player* player)
{
    // リセット
    player->GetPart("Head")->m_rotation = Quaternion::Identity;
    player->GetPart("Axe")->m_rotation = Quaternion::Identity;
}
