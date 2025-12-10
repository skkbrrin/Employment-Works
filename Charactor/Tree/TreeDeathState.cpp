#include "pch.h"
#include "TreeDeathState.h"
#include "Tree.h"

using namespace DirectX::SimpleMath;

void TreeDeathState::Enter(Tree* tree)
{
    m_timer = 0.0f;
}

void TreeDeathState::Update(Tree* tree, float elapsedTime)
{
    m_timer += elapsedTime;
    
    if (m_timer >= 1.0f)
    {
        tree->RequestDrop(); 
        tree->SetDeleteFlag(true);
    }
}

void TreeDeathState::Exit(Tree* tree)
{
}
