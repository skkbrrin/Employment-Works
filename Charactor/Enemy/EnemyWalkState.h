#pragma once
#include "EnemyState.h"

class EnemyWalkState : public EnemyState
{
private:
    float m_timer = 0.0f;
public:
    void Enter(Enemy* enemy) override;
    void Update(Enemy* enemy, float elapsedTime) override;
    void Exit(Enemy* enemy) override;
};
