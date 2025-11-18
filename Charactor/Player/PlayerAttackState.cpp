#include "pch.h"
#include "PlayerAttackState.h"
#include "PlayerIdleState.h"
#include "Player.h"

using namespace DirectX::SimpleMath;

void PlayerAttackState::Enter(Player* player)
{
    m_timer = 0.0f;
}

void PlayerAttackState::Update(Player* player, float elapsedTime)
{
    m_timer += elapsedTime;

    float swing = sinf(m_timer * 10.0f);
    auto head = player->GetPart("Head");
    auto axe = player->GetPart("Axe");

    // 攻撃アニメーション
    head->m_rotation = Quaternion::CreateFromAxisAngle(Vector3::UnitY, swing * 0.8f);
    //axe->m_rotation = Quaternion::CreateFromAxisAngle(Vector3::UnitZ, swing * 0.5f);



    // -------------- ここから ↓ 攻撃判定 ------------------
    //if (m_timer > 0.15f && m_timer < 0.35f) // 振り抜き中だけ攻撃判定ON
    //{
    //    // 武器のワールド座標取得
    //    Matrix weaponWorld = weapon->GetWorldMatrix();
    //    Vector3 hitPos = weaponWorld.Translation();

    //    // Sphere コライダー（半径は調整）
    //    float weaponRadius = 0.3f;

    //    Sphere weaponHit(hitPos, weaponRadius);

    //    // 全ての敵に対して判定
    //    for (auto enemy : EnemyManager::Instance().GetEnemies())
    //    {
    //        if (weaponHit.Intersects(enemy->GetCollider()))
    //        {
    //            enemy->TakeDamage(10);
    //            enemy->OnHitEffect(); // ノックバックなどしたければ
    //        }
    //    }
    //}
    // -----------------------------------------------------

    // 攻撃終了
    if (m_timer > 0.65f)
    {
        player->ChangeState(std::make_unique<PlayerIdleState>());
    }
}

void PlayerAttackState::Exit(Player* player)
{
    // リセット
    player->GetPart("Head")->m_rotation = Quaternion::Identity;
    player->GetPart("Axe")->m_rotation = Quaternion::Identity;
}
