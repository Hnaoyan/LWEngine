#include "../StateMachine.h"
#include "Application/GameObject/GameObjectLists.h"

void BossState::WaitState::Initialize()
{
	boss_->SetNowVariantState(this);

	changeTimer_.Start(60.0f);
}

void BossState::WaitState::Update()
{
	RotateUpdate();

	TimerUpdate(this);
}

void BossState::WaitState::Exit()
{
	boss_->SetPrevVariantState(this);
}
