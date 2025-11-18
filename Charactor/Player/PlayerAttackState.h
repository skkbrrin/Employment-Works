#pragma once
#include "PlayerState.h"

class PlayerAttackState : public PlayerState
{
private:
    float m_timer = 0.0f;
public:
    void Enter(Player* player) override;
    void Update(Player* player, float elapsedTime) override;
    void Exit(Player* player) override;
};
