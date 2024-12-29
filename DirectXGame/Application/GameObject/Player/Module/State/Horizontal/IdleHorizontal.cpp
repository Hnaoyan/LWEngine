#include "IdleHorizontal.h"
#include "Application/GameObject/Player/Player.h"
#include "Application/GameSystem/GameSystem.h"

void IdleHorizontal::Initialize()
{
	nowState_ = this;
	player_->acceleration_ = {};
}

void IdleHorizontal::Update()
{
	if (isLeftStickActive_) {
		stateMachine_->ChangeRequest(PlayerStateLists::kMove);
		return;
	}
}

void IdleHorizontal::Exit()
{

}

void IdleHorizontal::InputHandle()
{
	// 入力
	IPlayerState::InputHandle();

}
