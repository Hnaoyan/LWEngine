#include "IdleState.h"
#include "../../Player.h"

void IdleState::Initialize()
{

}

void IdleState::Update()
{

}

void IdleState::Exit()
{

}

void IdleState::InputHandle()
{
	if (input_->TriggerKey(DIK_L)) {
		stateManager_->ChangeRequest(StateManager::kJump);
	}
}
