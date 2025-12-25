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

class Player : public CharacterBase, public IHitBoxProvider
{
public:
    Player();
    ~Player();
    void ChangeState(std::unique_ptr<PlayerState> newState);
    
    void Initialize(ID3D11Device* device);
    void Update(float dt);
    void RenderP(ID3D11DeviceContext* ctx,
        DirectX::CommonStates* states,
        DirectX::SimpleMath::Matrix view,
        DirectX::SimpleMath::Matrix proj);

    void MoveForward(float dist);
    void RotateY(float deg);

    DirectX::SimpleMath::Vector3 GetForward() const
    {
        // Z+ ‚ğ‘O‚Æ‚·‚é
        return DirectX::SimpleMath::Vector3::Transform(
            DirectX::SimpleMath::Vector3::UnitZ,
            m_rotation
        );
    }

    // UŒ‚ƒqƒbƒgƒ{ƒbƒNƒX
    std::vector<HitBoxPart> GetHitBoxes() const override;

    void TakeDamage(float dmg, const DirectX::SimpleMath::Vector3& attackerPos);

    void AddWood(int value) {
        woodCount += value;
        std::cout << "Field Player AddWood! count=" << woodCount << std::endl;
    }
    int GetWoodCount() const { return woodCount; }
    DirectX::SimpleMath::Vector3& GetPosition() { return m_position; }
    DirectX::SimpleMath::Quaternion& GetRotation() { return m_rotation; }
    void SetPosition(DirectX::SimpleMath::Vector3& pos) { m_position = pos; }
    void SetRotation(const DirectX::SimpleMath::Quaternion& rot) { m_rotation = rot; }

    int GetMaxHP() { return m_maxHP; }
    
    // ”ÍˆÍUŒ‚’†‚©‚Ç‚¤‚©
    bool GetPowerAttacking() const { return m_isPowerAttacking; }
    void SetPowerAttacking(bool isattack) { m_isPowerAttacking = isattack; }
    
    // ”ÍˆÍUŒ‚‚Ì”¼Œa
    void SetPowerAttackRadius(float r) { m_powerAttackRadius = r; }
    float GetPowerAttackRadius() const { return m_powerAttackRadius; }

    // ƒ_ƒbƒVƒ…UŒ‚’†‚©‚Ç‚¤‚©
    bool GetDashAttacking() const { return m_isDashAttacking; }
    void SetDashAttacking(bool v) { m_isDashAttacking = v; }
private:
    std::unique_ptr<PlayerState> m_state;
    std::unique_ptr<TransformNode> m_root;

    int woodCount = 0;
    int m_maxHP = 90;

    DirectX::SimpleMath::Vector3 m_NockBackVelocity = DirectX::SimpleMath::Vector3::Zero;
    bool m_isNockBack = false;

    // ”ÍˆÍUŒ‚’†‚©‚Ç‚¤‚©
    bool m_isPowerAttacking = false;

    // ”ÍˆÍUŒ‚‚Ì”¼Œa
    float m_powerAttackRadius = 0.0f;

    // ƒ_ƒbƒVƒ…UŒ‚’†‚©‚Ç‚¤‚©
    bool m_isDashAttacking = false;
};

