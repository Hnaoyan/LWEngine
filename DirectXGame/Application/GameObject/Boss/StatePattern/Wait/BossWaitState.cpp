#include "../StateMachine.h"
#include "Application/GameObject/GameObjectLists.h"

void BossState::WaitState::Initialize()
{
	boss_->SetNowVariantState(this);
	float hpThreshold = 1.0f / 2.0f;
	float changeFrame = 30.0f;
	if (boss_->GetHealth()->GetHPRatio() < hpThreshold) {
		changeTimer_.Start(changeFrame);
	}
	else {
		changeFrame = 50.0f;
		changeTimer_.Start(changeFrame);
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
