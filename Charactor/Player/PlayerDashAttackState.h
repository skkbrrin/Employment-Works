#pragma once
#include "PlayerState.h"

class PlayerDashAttackState : public PlayerState
{
private:
    float m_timer = 0.0f;
    DirectX::SimpleMath::Vector3 m_dashDir = DirectX::SimpleMath::Vector3::Zero;

    DirectX::Keyboard::State m_kb;
    DirectX::Keyboard::KeyboardStateTracker m_kbTracker;

public:
    void Enter(Player* player) override;
    void Update(Player* player, float elapsedTime) override;
    void Exit(Player* player) override;
};
