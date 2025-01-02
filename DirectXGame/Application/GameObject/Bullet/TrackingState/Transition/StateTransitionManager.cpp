#include "StateTransitionManager.h"
#include <cassert>

void StateTransitionManager::Initialize(BulletStateMachine* stateMachine)
{
	assert(stateMachine);
	// マシンの設定
	stateMachine_ = stateMachine;

}

void StateTransitionManager::Update()
{
	transitionTimer_.Update();

}
