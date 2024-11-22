#include "StateManager.h"
#include "../../Player.h"
#include <cassert>

void PlayerStateManager::Initialize(Player* player)
{
	assert(player);
	player_ = player;

	// 軸ごとのステート
	verticalState_ = std::make_unique<PlayerStateMachine>();
	verticalState_->Initialize(player);
	verticalState_->ChangeRequest(PlayerStateLists::kIdleVertical);
	verticalState_->Update();
	horizontalState_ = std::make_unique<PlayerStateMachine>();
	horizontalState_->Initialize(player);
	horizontalState_->ChangeRequest(PlayerStateLists::kIdleHorizontal);
	horizontalState_->Update();

}

void PlayerStateManager::Update()
{
	horizontalState_->Update();
	verticalState_->Update();
}
