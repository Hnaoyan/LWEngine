#include "../StateMachine.h"
#include "Application/GameObject/GameObjectLists.h"

void BossState::WaitState::Initialize()
{
	changeTimer_.Start(60.0f);
}

void BossState::WaitState::Update()
{
	TimerUpdate(this);
}

void BossState::WaitState::Exit()
{
	boss_->SetPrevVariantState(this);
	//boss_->prevVariantState_ = this;
}
