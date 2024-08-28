#include "IdleState.h"
#include "../../Player.h"

void IdleState::Initialize()
{
	nowState_ = this;
}

void IdleState::Update()
{
	//// ステート変更
	//if (!player_->isGround_) {
	//	stateManager_->ChangeRequest(StateManager::kFall);
	//	return;
	//}

}

void IdleState::Exit()
{

}

void IdleState::InputHandle()
{
	//if (input_->TriggerKey(DIK_L)) {
	//	stateManager_->ChangeRequest(StateManager::kJump);
	//}
}
