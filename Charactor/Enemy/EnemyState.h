#pragma once
class Enemy;

class EnemyState
{
public:
    virtual ~EnemyState() = default;
    virtual void Enter(Enemy* enemy) {}
    virtual void Update(Enemy* enemy, float elapsedTime) = 0;
    virtual void Exit(Enemy* enemy) {}
};
