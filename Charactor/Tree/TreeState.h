#pragma once
class Tree;

class TreeState
{
public:
    virtual ~TreeState() = default;
    virtual void Enter(Tree* tree) {}
    virtual void Update(Tree* tree, float elapsedTime) = 0;
    virtual void Exit(Tree* tree) {}
};
