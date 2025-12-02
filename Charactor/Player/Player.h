#pragma once
#include <memory>
#include <unordered_map>
#include <string>
#include <SimpleMath.h>
#include "Charactor/CharactorBase.h"
#include "PlayerState.h"
#include "Charactor/IHitBoxProvider.h"
#include <Charactor/TransformNode.h>

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

    // 攻撃ヒットボックス
    std::vector<HitBoxPart> GetHitBoxes() const override;

    void AddWood(int value) { woodCount += value; }
    int GetWoodCount() const { return woodCount; }

private:
    std::unique_ptr<PlayerState> m_state;
    std::unique_ptr<TransformNode> m_root;

    int woodCount = 0;


};

