#include "../StateMachine.h"
#include "Application/GameObject/GameObjectLists.h"

void BossState::WaitState::Initialize()
{
	boss_->SetNowVariantState(this);
	if (boss_->GetHealth()->GetHPRatio() < 0.5f) {
		changeTimer_.Start(30.0f);
	}
	else {
		changeTimer_.Start(50.0f);
	}
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
