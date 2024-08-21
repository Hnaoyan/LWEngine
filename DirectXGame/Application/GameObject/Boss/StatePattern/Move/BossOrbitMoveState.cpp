#include "../StateMachine.h"
#include "Application/GameObject/GameObjectLists.h"

void BossState::OrbitMoveState::Initialize()
{

}

void BossState::OrbitMoveState::Update()
{

}

void BossState::OrbitMoveState::Exit()
{
	boss_->SetPrevVariantState(this);
}

void BossState::OrbitMoveState::GenerateMovePoint(float length, QuaterRotationPattern pattern) {
	length, pattern;
}