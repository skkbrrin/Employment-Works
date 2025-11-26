#pragma once
#include <SimpleMath.h>
#include <string>
#include <unordered_map>
#include <vector>
#include "TransformNode.h"

class CharacterBase
{
public:
    CharacterBase() {}
    virtual ~CharacterBase() {}

    DirectX::SimpleMath::Vector3 m_position = { 0,0,0 };
    DirectX::SimpleMath::Quaternion m_rotation =
        DirectX::SimpleMath::Quaternion::Identity;

    // HP
    int GetHP() const { return m_hp; }
    virtual void TakeDamage(int dmg) { m_hp -= dmg; }

    // ワールド行列
    DirectX::SimpleMath::Matrix GetWorldMatrix() const
    {
        using namespace DirectX::SimpleMath;
        return Matrix::CreateFromQuaternion(m_rotation) *
            Matrix::CreateTranslation(m_position);
    }

    // パーツ取得
    TransformNode* GetPart(const std::string& name) const
    {
        auto it = m_parts.find(name);
        return (it != m_parts.end()) ? it->second : nullptr;
    }

    // 更新
    virtual void Update(float dt) = 0;

protected:
    int m_hp = 10;
    std::unordered_map<std::string, TransformNode*> m_parts;
};
