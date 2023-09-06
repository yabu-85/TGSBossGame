#pragma once

class Player;
class PlayerState;

class PlayerStateManager
{
public:
    PlayerStateManager(Player& player) : player_(player) {}

    void ChangeState(PlayerState* newState);
    void Update();

private:
    Player& player_;
    PlayerState* currentState_ = nullptr;
};
