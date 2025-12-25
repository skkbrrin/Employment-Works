#pragma once
#include "PlayerState.h"

class PlayerWalkState : public PlayerState
{
private:
    float m_timer = 0.0f;
    float m_movedist = 0.05f;
    float m_legSpeed = 20.0f;

    DirectX::Keyboard::KeyboardStateTracker m_kbTracker;
public:
    void Enter(Player* player) override;
    void Update(Player* player, float elapsedTime) override;
    void Exit(Player* player) override;
};
