#pragma once
#include "TreeState.h"

class TreeDeathState : public TreeState
{
private:
    float m_timer = 0.0f;
public:
    void Enter(Tree* tree) override;
    void Update(Tree* tree, float elapsedTime) override;
    void Exit(Tree* tree) override;
};
