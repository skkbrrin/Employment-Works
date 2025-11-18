#pragma once
class Player;

class PlayerState
{
public:
    virtual ~PlayerState() = default;
    virtual void Enter(Player* player) {}
    virtual void Update(Player* player, float elapsedTime) = 0;
    virtual void Exit(Player* player) {}
};
