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
	changeTimer_.Start(90.0f);
	fireTimer_.Start(10.0f);
}

void BossState::MissileAttackState::Update()
{
	if (!fireTimer_.IsActive()) {
		EulerTransform pos = boss_->worldTransform_.transform_;
		pos.scale = { 1.0f,1.0f,1.0f };
		Vector3 moveDirect = { 0,0,-1.0f };
		boss_->GetBulletManager()->GetBeginCluster()->AddBullet(pos, moveDirect,50.0f);
		fireTimer_.Start(10.0f);
	}
	fireTimer_.Update();

	changeTimer_.Update();
	// 終了時に変更
	if (changeTimer_.IsEnd()) {
		boss_->StateManager()->ChangeRequest(std::make_unique<MoveState>());
		return;
	}
}

void BossState::MissileAttackState::Exit()
{

}

void BossState::MoveState::Initialize()
{
	changeTimer_.Start(300.0f);
}

void BossState::MoveState::Update()
{
	// とりあえずの左右移動
	if (isLeft_) {
		if (boss_->respawnPos_.x - 50.0f > boss_->worldTransform_.GetWorldPosition().x) {
			isLeft_ = false;
		}
		else {
			boss_->worldTransform_.transform_.translate.x -= 0.1f;
		}
	}
	else {
		if (boss_->respawnPos_.x + 50.0f < boss_->worldTransform_.GetWorldPosition().x) {
			isLeft_ = true;
		}
		else {
			boss_->worldTransform_.transform_.translate.x += 0.1f;
		}
	}

	changeTimer_.Update();
	// 終了時に変更
	if (changeTimer_.IsEnd()) {
		boss_->StateManager()->ChangeRequest(std::make_unique<MissileAttackState>());
		return;
	}
}

void BossState::MoveState::Exit()
{
	isLeft_ = false;
}
