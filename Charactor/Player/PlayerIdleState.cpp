#include "pch.h"
#include "PlayerIdleState.h"
#include "Player.h"
#include "PlayerWalkState.h"
#include "PlayerAttackState.h"

using namespace DirectX::SimpleMath;

void PlayerIdleState::Enter(Player* player)
{
    m_timer = 0.0f;
}

void PlayerIdleState::Update(Player* player, float elapsedTime)
{
    auto kb = DirectX::Keyboard::Get().GetState();

    // タイマー継続
    m_timer += elapsedTime;

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

    // しっぽを左右に振る（振れ幅は小さくした方が自然）
    float angle = sinf(m_timer * 5.0f) * DirectX::XMConvertToRadians(5.0f);
    player->GetPart("Tail")->m_rotation =
        Quaternion::CreateFromAxisAngle(Vector3::UnitZ, angle);

    // 歩きへ
    if (kb.W || kb.A || kb.D || kb.S)
    {
        player->ChangeState(std::make_unique<PlayerWalkState>());
        return;
    }

    // 攻撃へ
    if (kb.Space)
    {
        player->ChangeState(std::make_unique<PlayerAttackState>());
        return;
    }
}

void PlayerIdleState::Exit(Player* player)
{
}
