#pragma once
#include <memory>
#include <unordered_map>
#include <string>
#include <SimpleMath.h>
#include "Charactor/CharactorBase.h"
#include "PlayerState.h"
#include "Charactor/IHitBoxProvider.h"
#include <Charactor/TransformNode.h>
#include <iostream>

#include "Charactor/UI/DashParticle.h"
#include "Charactor/UI/DustParticle.h"
#include "Charactor/UI/WindParticle.h"

enum class CameraRequest
{
    None,
    SpinPrepare,
    SpinMain,
    SpinEnd,
};

class Player : public CharacterBase, public IHitBoxProvider
{
public:
    Player();
    ~Player();
    void ChangeState(std::unique_ptr<PlayerState> newState);
    
    void Initialize(ID3D11Device* device, DX::DeviceResources* dr);
    void Update(float dt);
    void RenderP(ID3D11DeviceContext* ctx,
        DirectX::CommonStates* states,
        DirectX::SimpleMath::Matrix view,
        DirectX::SimpleMath::Matrix proj);

    void MoveForward(float dist);
    void RotateY(float deg);
    void Jump();

    // 移動
    DirectX::SimpleMath::Vector3 GetForward() const
    {
        // Z+ を前とする
        return DirectX::SimpleMath::Vector3::Transform(
            DirectX::SimpleMath::Vector3::UnitZ,
            m_rotation
        );
    }

    // 右方向の取得
    DirectX::SimpleMath::Vector3 GetRight() const
    {
        // 回転から右方向を取得
        return DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3::UnitX, m_rotation);
    }

    // 攻撃ヒットボックス
    std::vector<HitBoxPart> GetHitBoxes() const override;

    void TakeDamage(float dmg, const DirectX::SimpleMath::Vector3& attackerPos);

    DirectX::SimpleMath::Vector3& GetPosition() { return m_position; }
    DirectX::SimpleMath::Quaternion& GetRotation() { return m_rotation; }
    void SetPosition(DirectX::SimpleMath::Vector3& pos) { m_position = pos; }
    void SetRotation(const DirectX::SimpleMath::Quaternion& rot) { m_rotation = rot; }

    int GetMaxHP() { return m_maxHP; }
    
    // 範囲攻撃中かどうか
    bool GetPowerAttacking() const { return m_isPowerAttacking; }
    void SetPowerAttacking(bool isattack) { m_isPowerAttacking = isattack; }
    
    // 範囲攻撃の半径
    void SetPowerAttackRadius(float r) { m_powerAttackRadius = r; }
    float GetPowerAttackRadius() const { return m_powerAttackRadius; }

    // 無敵時間
    bool IsInvincible() const { return m_invincibleTimer > 0.0f; }
    bool IsBlink() const
    {
        return IsInvincible() && ((int)(m_invincibleTimer * 10) % 2 == 0);
    }

    // パーティクル
    DashParticle* GetDashParticle() { return m_dashParticle.get(); }
    WindParticle* GetSpinWindParticle() { return m_spinWindParticle.get(); }
    DustParticle* GetSpinDustParticle() { return m_spinDustParticle.get(); }

    // カメラ
    void RequestCamera(CameraRequest req)
    {
        m_cameraRequest = req;
    }
    CameraRequest GetCameraRequest() { return m_cameraRequest; }
private:
    std::unique_ptr<PlayerState> m_state;
    std::unique_ptr<TransformNode> m_root;

    int woodCount = 0;
    int m_maxHP = 3;

    DirectX::SimpleMath::Vector3 m_NockBackVelocity = DirectX::SimpleMath::Vector3::Zero;
    bool m_isNockBack = false;

    // 範囲攻撃中かどうか
    bool m_isPowerAttacking = false;

    // 範囲攻撃の半径
    float m_powerAttackRadius = 0.0f;

    // ジャンプ
    bool m_isJumping = false;
    float m_jumpVelocity = 0.0f;

    // 無敵時間
    float m_invincibleTimer = 0.0f;
    const float INVINCIBLE_TIME = 2.0f;

    // パーティクル
    std::unique_ptr<DashParticle> m_dashParticle;
    std::unique_ptr<WindParticle> m_spinWindParticle;
    std::unique_ptr<DustParticle> m_spinDustParticle;

    // カメラ
    CameraRequest m_cameraRequest = CameraRequest::None;
};

