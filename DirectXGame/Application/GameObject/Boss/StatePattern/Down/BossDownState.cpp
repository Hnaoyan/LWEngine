#include "../StateMachine.h"
#include "Application/GameObject/GameObjectLists.h"

void BossState::SystemDownState::Initialize()
{
	boss_->SetNowVariantState(this);
	changeTimer_.Start(60.0f);
}

void BossState::SystemDownState::Update()
{
	TimerUpdate(this);
}

void BossState::SystemDownState::Exit()
{
	boss_->SetPrevVariantState(this);
}
