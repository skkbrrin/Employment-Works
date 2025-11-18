#pragma once
#include <SimpleMath.h>
#include <string>
#include <unordered_map>
#include "TransformNode.h"

class CharacterBase
{
public:
    CharacterBase() {}
    virtual ~CharacterBase() {}

    DirectX::SimpleMath::Vector3 m_position = { 0,0,0 };
    DirectX::SimpleMath::Quaternion m_rotation = DirectX::SimpleMath::Quaternion::Identity;

    TransformNode* GetPart(const std::string& name) const
    {
        auto it = m_parts.find(name);
        return (it != m_parts.end()) ? it->second : nullptr;
    }

    virtual void Update(float dt) = 0;

    virtual void TakeDamage(int dmg) {
        m_hp -= dmg;
    }

    // ====== AABB の安全な取得 ======
    virtual DirectX::BoundingBox GetAABB() const
    {
        using namespace DirectX;

        BoundingBox box;

        // 安全に Body を取得
        const TransformNode* body = nullptr;
        auto it = m_parts.find("Body");
        if (it == m_parts.end())
        {
            // パーツがない場合はデフォルト AABB
            BoundingBox::CreateFromPoints(
                box,
                SimpleMath::Vector3(-0.5f, -0.5f, -0.5f),
                SimpleMath::Vector3(0.5f, 0.5f, 0.5f)
            );
            return box;
        }
        body = it->second;

        // Body のローカルAABB（キャラごとに調整可）
        BoundingBox localBox;
        BoundingBox::CreateFromPoints(
            localBox,
            SimpleMath::Vector3(-0.3f, -0.4f, -0.2f),
            SimpleMath::Vector3(0.3f, 0.4f, 0.2f)
        );

        // world行列で変換
        localBox.Transform(box, body->GetWorldMatrix());
        return box;
    }

    int GetHP() const { return m_hp; }

protected:
    int m_hp = 100;
    std::unordered_map<std::string, TransformNode*> m_parts;
};
