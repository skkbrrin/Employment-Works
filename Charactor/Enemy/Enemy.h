#pragma once
#include <memory>
#include <vector>
#include <string>
#include <SimpleMath.h>

#include "Charactor/CharactorBase.h"
#include "Charactor/IHitBoxProvider.h"
#include "EnemyState.h"
#include "Charactor/TransformNode.h"

class Enemy : public CharacterBase, public IHitBoxProvider
{
public:
    Enemy();
    ~Enemy();

    void ChangeState(std::unique_ptr<EnemyState> newState);

    void Initialize(ID3D11Device* device);
    void Update(float dt);
    void RenderE(
        ID3D11DeviceContext* ctx,
        DirectX::CommonStates* states,
        DirectX::SimpleMath::Matrix view,
        DirectX::SimpleMath::Matrix proj);

    void MoveForward(float dist);
    void RotateY(float deg);

    void SetPosition(const DirectX::SimpleMath::Vector3& pos)
    {
        m_position = pos;
        if (m_root)
        {
            m_root->m_position = m_position;
            m_root->UpdateWorldMatrix();
        }
    }

    void SetRotation(const DirectX::SimpleMath::Quaternion& rot)
    {
        m_rotation = rot;
        if (m_root)
        {
            m_root->m_rotation = m_rotation;
            m_root->UpdateWorldMatrix();
        }
    }

    std::vector<DirectX::BoundingOrientedBox>
        GetLocalHitBoxes() const override;

    std::vector<DirectX::BoundingOrientedBox>
        GetWorldHitBoxes() const override;

private:
    std::unique_ptr<EnemyState> m_state;

    std::unique_ptr<TransformNode> m_root;

    DirectX::SimpleMath::Vector3     m_position = { 0,0,0 };
    DirectX::SimpleMath::Quaternion  m_rotation = DirectX::SimpleMath::Quaternion::Identity;

    bool m_isAttacking = false;
};
