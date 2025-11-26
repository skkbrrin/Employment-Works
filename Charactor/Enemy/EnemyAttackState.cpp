#include "pch.h"
#include "EnemyAttackState.h"
#include "EnemyIdleState.h"
#include "Enemy.h"

using namespace DirectX::SimpleMath;

void EnemyAttackState::Enter(Enemy* enemy)
{
    m_timer = 0.0f;
}

void EnemyAttackState::Update(Enemy* enemy, float elapsedTime)
{
    m_timer += elapsedTime;

    // 攻撃アニメーション
    float pitch = abs(sinf(m_timer * 4) * 0.6f); 
    float roll = abs(sinf(m_timer * 4) * 0.3f);

    enemy->GetPart("LegR")->m_rotation =
        Quaternion::CreateFromYawPitchRoll(0, pitch, roll);

    // --------------  攻撃判定 ---------------------------
    //if (m_timer > 0.15f && m_timer < 0.55f)
    //{
    //    Matrix weaponWorld = weapon->GetWorldMatrix();
    //    Vector3 hitPos = weaponWorld.Translation();

    //    // Sphere コライダー
    //    float weaponRadius = 0.3f;

    //    Sphere weaponHit(hitPos, weaponRadius);

    //    // プレイヤー判定
    //    if (weaponHit.Intersects(player->GetCollider()))
    //    {
    //        player->TakeDamage(10);
    //        player->OnHitEffect();
    //    }

    // 攻撃終了
    /*if (m_timer > 0.65f)
    {
        enemy->ChangeState(std::make_unique<EnemyIdleState>());
    }*/
}

void EnemyAttackState::Exit(Enemy* enemy)
{
    //// リセット
    //enemy->GetPart("Head")->m_rotation = Quaternion::Identity;
    //enemy->GetPart("Axe")->m_rotation = Quaternion::Identity;
}
