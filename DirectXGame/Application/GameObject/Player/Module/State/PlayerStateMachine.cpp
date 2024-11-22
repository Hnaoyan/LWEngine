#include "PlayerStateMachine.h"
#include "../../Player.h"
#include <cassert>

void PlayerStateMachine::Initialize(Player* player)
{
	assert(player);
	player_ = player;
}

void PlayerStateMachine::Update()
{
	if (request_) {
		// 元の終了
		if (currentState_) {
			currentState_->Exit();
		}

		// 新しいやつの初期化
		tmpState_->PreInitialize(player_, this);
		tmpState_->Initialize();
		currentState_ = std::move(tmpState_);

		// リクエストリセット
		request_ = std::nullopt;
	}
	if (currentState_) {
		currentState_->InputHandle();
		currentState_->Update();
	}
}

void PlayerStateMachine::ChangeRequest(PlayerStateLists request)
{
	request_ = request;

	std::unique_ptr<IPlayerState> newState;
	switch (request)
	{
	case PlayerStateLists::kIdleVertical:
		newState = std::make_unique<IdleVertical>();
		break;
	case PlayerStateLists::kIdleHorizontal:
		newState = std::make_unique<IdleHorizontal>();
		break;
	case PlayerStateLists::kMove:
		newState = std::make_unique<MovingState>();
		break;
	case PlayerStateLists::kJump:
		newState = std::make_unique<JumpingState>();
		break;
	case PlayerStateLists::kFall:
		newState = std::make_unique<FallingState>();
		break;
	case PlayerStateLists::kBoost:
		newState = std::make_unique<BoostState>();
		break;
	case PlayerStateLists::kQuickBoost:
		newState = std::make_unique<QuickBoostState>();
		break;
	case PlayerStateLists::kAssending:
		newState = std::make_unique<AssendingState>();
		break;
	}

	tmpState_ = std::move(newState);
}
