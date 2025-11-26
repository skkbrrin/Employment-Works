#include "pch.h"
#include "EnemyIdleState.h"
#include "Enemy.h"
#include "EnemyWalkState.h"
#include "EnemyAttackState.h"

using namespace DirectX::SimpleMath;

void EnemyIdleState::Enter(Enemy* enemy)
{
    m_timer = 0.0f;
}

void EnemyIdleState::Update(Enemy* enemy, float elapsedTime)
{
    auto kb = DirectX::Keyboard::Get().GetState();

    // ƒ^ƒCƒ}[Œp‘±
    m_timer += elapsedTime;

    if (m_timer / 7 > 1 && m_timer / 7 < 1.3)
    {
        enemy->GetPart("Body")->m_rotation =
            Quaternion::CreateFromAxisAngle(Vector3::UnitY, sin(m_timer*9) / 8);
    }


    //// •à‚«‚Ö
    //if (kb.Up || kb.Down || kb.Left || kb.Right)
    //{
    //    enemy->ChangeState(std::make_unique<EnemyWalkState>());
    //    return;
    //}

    //// UŒ‚‚Ö
    //if (kb.Space)
    //{
    //    enemy->ChangeState(std::make_unique<EnemyAttackState>());
    //    return;
    //}
}

void EnemyIdleState::Exit(Enemy* enemy)
{
}
