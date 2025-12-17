#include "pch.h"
#include "EnemyAttackState.h"
#include "EnemyIdleState.h"
#include "Enemy.h"

using namespace DirectX::SimpleMath;
using namespace DirectX;


void EnemyAttackState::Enter(Enemy* enemy)
{
    m_timer = 0.0f;

    if (auto root = enemy->GetPart("Root"))
    {
        root->m_position = Vector3::Zero;
    }

    if (auto body = enemy->GetPart("Body"))
    {
        body->m_position = Vector3::Zero;
    }

    if (auto legR = enemy->GetPart("LegR"))
    {
        legR->m_position = Vector3::Zero;
    }

    if (auto legL = enemy->GetPart("LegL"))
    {
        legL->m_position = Vector3::Zero;
    }

}


void EnemyAttackState::Update(Enemy* enemy, float elapsedTime)
{
    m_timer += elapsedTime;

    // プレイヤー方向に向かう
    // プレイヤーと敵の場所を取る
    Vector3 playerPos = enemy->GetPlayerPosition();
    Vector3 enemyPos = enemy->GetPosition();
    Vector3 toPlayer = playerPos - enemyPos;

    // プレイヤー方向に回転
    float targetYaw = 0;
    targetYaw = atan2f(toPlayer.x, toPlayer.z);

    enemy->RotateY(targetYaw);

    // 前進方向
    Vector3 forward =
        Vector3::Transform(Vector3::UnitZ,
            Matrix::CreateFromQuaternion(enemy->GetRotation()));
    forward.Normalize();
    float moveSpeed = 1.0f; // 移動スピード

    // 場所の更新
    enemy->SetPosition(enemyPos + forward * moveSpeed * elapsedTime);

    // ------------------------------------------------------------------
    // 足を交互に切り替え
    if (m_timer >= 0.8f)
    {
        m_whichLeg = 1 - m_whichLeg;
        m_timer = 0;
    }

    // 足の振り
    float swing = abs(sinf(m_timer * 4.0f) * 1.5f);

    if (m_whichLeg == 0)
    {
        enemy->GetPart("LegR")->m_rotation = Quaternion::CreateFromYawPitchRoll(-swing, 0, 0);
        enemy->GetPart("LegR")->m_position = Vector3(0, -abs(sinf(m_timer * 4.0f) * 2.0f), 0);
        enemy->GetPart("LegL")->m_rotation = Quaternion::Identity;
        enemy->GetPart("LegL")->m_position = Vector3::Zero;
    }
    else
    {
        enemy->GetPart("LegL")->m_rotation = Quaternion::CreateFromYawPitchRoll(swing, 0, 0);
        enemy->GetPart("LegL")->m_position = Vector3(0, -abs(sinf(m_timer * 4.0f) * 2.0f), 0);
        enemy->GetPart("LegR")->m_rotation = Quaternion::Identity;
        enemy->GetPart("LegR")->m_position = Vector3::Zero;
    }

}

void EnemyAttackState::Exit(Enemy* enemy)
{
}
