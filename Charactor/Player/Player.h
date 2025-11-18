#pragma once
#include <memory>
#include <unordered_map>
#include <string>
#include <SimpleMath.h>
#include "Charactor/CharactorBase.h"
#include "PlayerState.h"

class Player : public CharacterBase
{
public:
    Player();
    ~Player();

    void Initialize(ID3D11Device* device);
    void Update(float dt) override;
    void RenderP(ID3D11DeviceContext* context,
        DirectX::CommonStates* states,
        DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);

    void ChangeState(std::unique_ptr<PlayerState> newState);

    void MoveForward(float dist);
    void RotateY(float deg);

    bool IsAttacking() const { return m_isAttacking; }
    DirectX::BoundingBox GetWeaponBox() const;

private:
    std::unique_ptr<PlayerState> m_state;
    std::unique_ptr<TransformNode> m_root;

    bool m_isAttacking = false;   // çUåÇíÜÉtÉâÉO
};

