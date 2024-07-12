#include "FallingState.h"
#include "../../Player.h"

void FallingState::Initialize()
{
}

void FallingState::Update()
{
	if (player_->isGround_) {
		stateManager_->ChangeRequest(StateManager::kIdle);
	}
}

void FallingState::Exit()
{
}

void FallingState::InputHandle()
{
}
