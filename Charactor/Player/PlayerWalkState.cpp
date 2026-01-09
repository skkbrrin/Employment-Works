#include "pch.h"
#include "PlayerWalkState.h"
#include "PlayerIdleState.h"
#include "PlayerAttackState.h"
#include "PlayerDashAttackState.h"
#include "Player.h"

using namespace DirectX::SimpleMath;

void PlayerWalkState::Enter(Player* player)
{
    m_timer = 0.0f;
    m_movedist = 0.1f;
    m_legSpeed = 20.0f;
}

void PlayerWalkState::Update(Player* player, float elapsedTime)
{
    m_timer += elapsedTime;
    auto kb = DirectX::Keyboard::Get().GetState();
    m_kbTracker.Update(kb);

    bool top = kb.W || kb.Up || kb.NumPad8;
    bool bottom = kb.S || kb.Down || kb.NumPad2;
    bool left = kb.A || kb.Left || kb.NumPad4;
    bool right = kb.D || kb.Right || kb.NumPad6;

    if (kb.Up)    OutputDebugStringA("Up\n");
    if (kb.Down)  OutputDebugStringA("Down\n");
    if (kb.Left)  OutputDebugStringA("Left\n");
    if (kb.Right) OutputDebugStringA("Right\n");

    // 移動処理
    if (top) player->MoveForward(m_movedist);
    if (left) player->RotateY(2.0f);
    if (right) player->RotateY(-2.0f);
    if (bottom) player->MoveForward(-m_movedist);

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
    if (m_kbTracker.pressed.Space)
    {
        player->ChangeState(std::make_unique<PlayerDashAttackState>());
        return;
    }
    if (!top && !left && !right && !bottom)
    {
        player->ChangeState(std::make_unique<PlayerIdleState>());
        return;
    }
}


void PlayerWalkState::Exit(Player* player)
{
}
