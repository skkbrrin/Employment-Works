#include "pch.h"
#include "EnemyDeathState.h"
#include "EnemyIdleState.h"
#include "EnemyAttackState.h"
#include "Enemy.h"

using namespace DirectX::SimpleMath;

void EnemyDeathState::Enter(Enemy* enemy)
{
    m_timer = 0.0f;
}

void EnemyDeathState::Update(Enemy* enemy, float elapsedTime)
{
    m_timer += elapsedTime;
    
    if (m_timer >= 1.0f)
    {
        enemy->RequestDrop(); 
        enemy->SetDeleteFlag(true);
    }
}


void EnemyDeathState::Exit(Enemy* enemy)
{
}
