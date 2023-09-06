#include "PlayerStateManager.h"
#include "Player.h"
#include "PlayerState.h"

void PlayerStateManager::ChangeState(PlayerState* newState)
{
    if (currentState_) {
        currentState_->Exit(player_);
    }
    newState->Enter(player_);
    currentState_ = newState;

}

void PlayerStateManager::Update()
{
    if (currentState_) {
        currentState_->Update(player_);
    }

}
