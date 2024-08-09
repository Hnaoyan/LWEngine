#include "StateMachine.h"
#include "Engine/LwLib/LwEngineLib.h"
#include "Application/GameObject/GameObjectLists.h"

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
		Vector3 moveDirect = Vector3::Normalize(boss_->GetPlayer()->worldTransform_.GetWorldPosition() - boss_->worldTransform_.GetWorldPosition());
		boss_->GetBulletManager()->GetBeginCluster()->AddBullet(pos, moveDirect,50.0f);
		fireTimer_.Start(10.0f);
	}
	// 射撃タイマー
	fireTimer_.Update();
	// 変更タイマー
	changeTimer_.Update();
	// 終了時に変更
	if (changeTimer_.IsEnd()) {
		boss_->GetDecider().StateDecide(this);
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
	// 変更タイマー
	changeTimer_.Update();
	// 終了時に変更
	if (changeTimer_.IsEnd()) {
		boss_->GetDecider().StateDecide(this);
		return;
	}
}

void BossState::MoveState::Exit()
{
	isLeft_ = false;
}

void BossState::MoveState::TestProcess()
{
	changeTimer_.Start(300.0f);
	isLeft_ = true;
}

void BossState::StateDecider::Initialize(Boss* boss, Player* player)
{
	// ポインタの設定
	boss_ = boss;
	player_ = player;
}

void BossState::StateDecider::StateDecide(StateVariant nowState)
{
	if (boss_->GetPrevState()) {
		// 攻撃中なら
		if (std::holds_alternative<MissileAttackState*>(nowState)) {
			boss_->StateManager()->ChangeRequest(std::make_unique<MoveState>());
			MoveState* newState = static_cast<MoveState*>(boss_->GetState());
			newState->TestProcess();
		}
	}
	else {
		// 移動中なら
		if (std::holds_alternative<MoveState*>(nowState)) {
			boss_->StateManager()->ChangeRequest(std::make_unique<MissileAttackState>());
		}
		// 攻撃中なら
		else if (std::holds_alternative<MissileAttackState*>(nowState)) {
			boss_->StateManager()->ChangeRequest(std::make_unique<MissileAttackState>());
		}
	}
}

void BossState::UpDownState::Initialize()
{
	float offset = 15.0f;
	if (boss_->worldTransform_.GetWorldPosition().y <= 0) {
		startPosition_ = boss_->worldTransform_.GetWorldPosition();
		endPosition_ = startPosition_;
		endPosition_.y += offset;
		isUpper_ = true;
	}
	else {
		startPosition_ = boss_->worldTransform_.GetWorldPosition();
		endPosition_ = startPosition_;
		endPosition_.y -= offset;
		isUpper_ = false;
	}
	// 変更のタイマー
	changeTimer_.Start(120.0f);

}

void BossState::UpDownState::Update()
{
	if (isUpper_) {
		//boss_->worldTransform_.transform_.translate = Vector3::Lerp()
	}

	//boss_->worldTransform_.transform_.translate = LwLib::Lerp(startPosition_,endPosition_,changeTimer_.)

	changeTimer_.Update();
	// 終了時に変更
	if (changeTimer_.IsEnd()) {
		boss_->GetDecider().StateDecide(this);
		return;
	}
}

void BossState::UpDownState::Exit()
{

}
