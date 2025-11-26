#include "pch.h"
#include "EnemyWalkState.h"
#include "EnemyIdleState.h"
#include "EnemyAttackState.h"
#include "Enemy.h"

using namespace DirectX::SimpleMath;

void EnemyWalkState::Enter(Enemy* enemy)
{
    m_timer = 0.0f;
}

void EnemyWalkState::Update(Enemy* enemy, float elapsedTime)
{
    m_timer += elapsedTime;
    auto kb = DirectX::Keyboard::Get().GetState();

    // 移動処理
    if (kb.Up) enemy->MoveForward(0.05f);
    if (kb.Left) enemy->RotateY(2.0f);
    if (kb.Right) enemy->RotateY(-2.0f);

    // ワキワキ
    enemy->GetPart("LegR")->m_rotation =
        Quaternion::CreateFromAxisAngle(Vector3::UnitX, sin(m_timer * 9) / 12);
    enemy->GetPart("LegL")->m_rotation =
        Quaternion::CreateFromAxisAngle(Vector3::UnitX, cos(m_timer * 9) / 12);

    // キーを離したらIdleに戻す
    if (kb.Space)
    {
        enemy->ChangeState(std::make_unique<EnemyAttackState>());
        return;
    }
}


void EnemyWalkState::Exit(Enemy* enemy)
{
}
