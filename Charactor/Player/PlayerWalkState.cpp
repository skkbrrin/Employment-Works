#include "pch.h"
#include "PlayerWalkState.h"
#include "PlayerIdleState.h"
#include "PlayerAttackState.h"
#include "Player.h"

using namespace DirectX::SimpleMath;

void PlayerWalkState::Enter(Player* player)
{
    m_timer = 0.0f;
}

void PlayerWalkState::Update(Player* player, float elapsedTime)
{
    m_timer += elapsedTime;
    auto kb = DirectX::Keyboard::Get().GetState();

    // 移動処理
    if (kb.Up) player->MoveForward(0.05f);
    if (kb.Left) player->RotateY(2.0f);
    if (kb.Right) player->RotateY(-2.0f);
    if (kb.Down) player->MoveForward(-0.08f);

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
    if (!kb.Up && !kb.Left && !kb.Right && !kb.Down)
    {
        player->ChangeState(std::make_unique<PlayerIdleState>());
        return;
    }
}


void PlayerWalkState::Exit(Player* player)
{
}
