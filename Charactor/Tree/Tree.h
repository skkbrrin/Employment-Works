#pragma once
#include <memory>
#include <vector>
#include <string>
#include <SimpleMath.h>

#include "Charactor/CharactorBase.h"
#include "Charactor/IHitBoxProvider.h"
#include "TreeState.h"
#include "Charactor/TransformNode.h"

class ItemManager;
class Player;

class Tree : public CharacterBase, public IHitBoxProvider
{
public:
    Tree();
    ~Tree();

    void ChangeState(std::unique_ptr<TreeState> newState);

    void Initialize(ID3D11Device* device);
    void Update(float dt) override;
    void RenderT(
        ID3D11DeviceContext* ctx,
        DirectX::CommonStates* states,
        DirectX::SimpleMath::Matrix view,
        DirectX::SimpleMath::Matrix proj);



    void SetPosition(const DirectX::SimpleMath::Vector3& pos)
    {
        m_position = pos;
        if (m_root)
        {
            m_root->m_position = m_position;
            m_root->UpdateWorldMatrix();
        }
    }
    DirectX::SimpleMath::Vector3& GetPosition() { return m_position; }

    void SetRotation(const DirectX::SimpleMath::Quaternion& rot)
    {
        m_rotation = rot;
        if (m_root)
        {
            m_root->m_rotation = m_rotation;
            m_root->UpdateWorldMatrix();
        }
    }
    DirectX::SimpleMath::Quaternion GetRotation() { return m_rotation; }

    DirectX::SimpleMath::Matrix GetWorldMatrix() const
    {
        return m_root ? m_root->GetWorldMatrix() : DirectX::SimpleMath::Matrix::Identity;
    }

    void SetDeleteFlag(bool v) { m_deleteFlag = v; }
    bool GetDeleteFlag() const { return m_deleteFlag; }

    std::vector<HitBoxPart> GetHitBoxes() const override;

    void TakeDamage(int dt) override;
    void Damaging();
    void Die();
    void SetItemManager(ItemManager* manager) { m_itemManager = manager; }
    void RequestDrop() { m_shouldDropItem = true; }
    bool ShouldDropItem() const { return m_shouldDropItem; }
    void SetPlayer(Player* player){ m_player = player; }

    DirectX::SimpleMath::Vector3& GetPlayerPosition();

private:
    std::unique_ptr<TreeState> m_state;

    std::unique_ptr<TransformNode> m_root;

    DirectX::SimpleMath::Vector3     m_position = { 0,0,0 };
    DirectX::SimpleMath::Quaternion  m_rotation = DirectX::SimpleMath::Quaternion::Identity;

    bool m_isAttacking = false;
    bool m_isAlive = true;
    bool m_deleteFlag = false;
    bool m_shouldDropItem = false;

    ItemManager* m_itemManager = nullptr;

    Player* m_player = nullptr;
};
