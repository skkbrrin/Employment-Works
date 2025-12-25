#pragma once
#include "PlayerState.h"

template <typename T>
T Clamp(T value, T minValue, T maxValue)
{
    if (value < minValue) return minValue;
    if (value > maxValue) return maxValue;
    return value;
}

inline float Lerp(float a, float b, float t)
{
    return a + (b - a) * t;
}

enum class SpinPhase
{
    Start,
    Spin,
    End
};

class PlayerPowerAttackState : public PlayerState
{
private:
    float m_timer = 0.0f;

    float m_angle = 0.0f;
    float m_startAngle = 0.0f;
    float m_endStartAngle = 0.0f;
    float m_anglarVelocity = 0.0f;

    SpinPhase m_phase = SpinPhase::Start;

    const float START_TIME = 1.0f;
    const float SPIN_TIME = 4.0f;
    const float END_TIME = 0.2f;

    float m_maxRadius = 30.0f;
public:
    void Enter(Player* player) override;
    void Update(Player* player, float elapsedTime) override;
    void UpdateStart(Player* player, float elapsedTime) ;
    void UpdateSpin(Player* player, float elapsedTime);
    void UpdateEnd(Player* player, float elapsedTime);
    void Exit(Player* player) override;
};
