#include "StateMachine.h"
#include "../TestBoss/Boss.h"

void BossState::StateManager::Initialize(Boss* boss)
{
	boss_ = boss;
}

void BossState::StateManager::ChangeRequest(std::unique_ptr<IState> newState)
{
	if (boss_->GetState()) {
		// 終了処理
		boss_->GetState()->Exit();
	}

	newState->PreInitialize(boss_);
	newState->Initialize();

	boss_->ChangeState(std::move(newState));
}

void BossState::IState::PreInitialize(Boss* boss)
{
	// ポインタ設定
	boss_ = boss;

}

void BossState::MissileAttackState::Initialize()
{

}

void BossState::MissileAttackState::Update()
{
	boss_->worldTransform_.transform_.translate.x += 0.1f;
}

void BossState::MissileAttackState::Exit()
{

}
