#pragma once
#include "PlayerState.h"

class PlayerIdleState : public PlayerState
{
private:
    float m_timer = 0.0f;
    int m_spaceCount;
    float m_typeTimer = 0.0f;

    DirectX::Keyboard::State m_kb;
    DirectX::Keyboard::KeyboardStateTracker m_kbTracker;

public:
    void Enter(Player* player) override;
    void Update(Player* player, float elapsedTime) override;
    void Exit(Player* player) override;

};
