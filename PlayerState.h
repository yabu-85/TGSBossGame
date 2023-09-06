#pragma once

class Player;

class PlayerState
{
public:
    virtual ~PlayerState() {}
    virtual void Enter(Player& player) {}
    virtual void Update(Player& player) = 0;
    virtual void Exit(Player& player) {}

};

