#include "pch.h"
#include "PlayerWalkState.h"
#include "PlayerIdleState.h"
#include "PlayerAttackState.h"
#include "Player.h"

using namespace DirectX::SimpleMath;

void PlayerWalkState::Enter(Player* player)
{
    m_timer = 0.0f;
    m_movedist = 0.05f;
    m_legSpeed = 20.0f;
}

void PlayerWalkState::Update(Player* player, float elapsedTime)
{
    m_timer += elapsedTime;
    auto kb = DirectX::Keyboard::Get().GetState();

    if (kb.LeftShift)
    {
        m_movedist = 1.0f;
        m_legSpeed = 40.0f;
    }

    // 移動処理
    if (kb.W) player->MoveForward(m_movedist);
    if (kb.A) player->RotateY(2.0f);
    if (kb.D) player->RotateY(-2.0f);
    if (kb.S) player->MoveForward(-m_movedist);

    // 歩行アニメーション用サイン波
    float swing = sinf(m_timer * 8.0f); // 周期早めにすると歩いてる感UP
    float angle = swing * DirectX::XMConvertToRadians(20.0f);

    // 前足
    player->GetPart("LegFrontL")->m_rotation =
        Quaternion::CreateFromAxisAngle(Vector3::UnitX, angle);
    player->GetPart("LegFrontR")->m_rotation =
        Quaternion::CreateFromAxisAngle(Vector3::UnitX, -angle);

    // 後足（前足と逆に動かす）
    player->GetPart("LegBackL")->m_rotation =
        Quaternion::CreateFromAxisAngle(Vector3::UnitX, -angle);
    player->GetPart("LegBackR")->m_rotation =
        Quaternion::CreateFromAxisAngle(Vector3::UnitX, angle);

    // しっぽも少し振ると可愛い
    float tailAngle = sinf(m_timer * 10.0f) * DirectX::XMConvertToRadians(15.0f);
    player->GetPart("Tail")->m_rotation =
        Quaternion::CreateFromAxisAngle(Vector3::UnitZ, tailAngle);

    // キーを離したらIdleに戻す
    if (kb.Space)
    {
        player->ChangeState(std::make_unique<PlayerAttackState>());
        return;
    }
    if (!kb.W && !kb.A && !kb.S && !kb.D)
    {
        player->ChangeState(std::make_unique<PlayerIdleState>());
        return;
    }
}


void PlayerWalkState::Exit(Player* player)
{
}
