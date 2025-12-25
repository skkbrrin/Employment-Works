#include "pch.h"
#include "PlayerIdleState.h"
#include "Player.h"
#include "PlayerWalkState.h"
#include "PlayerAttackState.h"

using namespace DirectX::SimpleMath;

void PlayerIdleState::Enter(Player* player)
{
    m_timer = 0.0f;
    m_typeTimer = 0.0f;
}

void PlayerIdleState::Update(Player* player, float elapsedTime)
{
    m_kb = DirectX::Keyboard::Get().GetState();
    m_kbTracker.Update(m_kb);

    // タイマー継続
    m_timer += elapsedTime;

    char buf[64];
    sprintf_s(buf, "SpaceCount = %d\n", (int)m_spaceCount);
    OutputDebugStringA(buf);

    // 前足
    player->GetPart("LegFrontL")->m_rotation =
        Quaternion::CreateFromAxisAngle(Vector3::UnitX, 0);
    player->GetPart("LegFrontR")->m_rotation =
        Quaternion::CreateFromAxisAngle(Vector3::UnitX, 0);

    // 後足
    player->GetPart("LegBackL")->m_rotation =
        Quaternion::CreateFromAxisAngle(Vector3::UnitX, 0);
    player->GetPart("LegBackR")->m_rotation =
        Quaternion::CreateFromAxisAngle(Vector3::UnitX, 0);

    // しっぽ
    float angle = sinf(m_timer * 5.0f) * DirectX::XMConvertToRadians(sinf(m_timer) * 10.0f);
    player->GetPart("Tail")->m_rotation =
        Quaternion::CreateFromAxisAngle(Vector3::UnitZ, angle);

    // 歩きへ
    if (m_kb.W || m_kb.A || m_kb.D || m_kb.S)
    {
        player->ChangeState(std::make_unique<PlayerWalkState>());
        return;
    }

    if (m_kbTracker.pressed.Space)
    {
        player->ChangeState(std::make_unique<PlayerAttackState>());
        return;
    }
}

void PlayerIdleState::Exit(Player* player)
{
}
