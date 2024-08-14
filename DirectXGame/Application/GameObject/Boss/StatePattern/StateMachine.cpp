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
void BossState::StateDecider::Initialize(Boss* boss, Player* player)
{
	// ポインタの設定
	boss_ = boss;
	player_ = player;
}

void BossState::StateDecider::StateDecide(StateVariant nowState)
{
	float distance = Vector3::Distance(boss_->worldTransform_.GetWorldPosition(), player_->worldTransform_.GetWorldPosition());

	if (distance >= 100.0f) {
		boss_->StateManager()->ChangeRequest(std::make_unique<MoveState>());
		MoveState* newState = static_cast<MoveState*>(boss_->GetState());
		newState->MoveSelect(player_->worldTransform_.GetWorldPosition());
		newState->TestProcess();
		return;
	}

	// ステートが変わる部分（ここで変更先の分岐
	if (boss_->GetPrevState()) {
		DefaultLoop(nowState);
	}
	else {
		// 移動状態
		if (std::holds_alternative<MoveState*>(nowState)) {
			boss_->StateManager()->ChangeRequest(std::make_unique<UpDownState>());
		}
		// 攻撃状態
		else if (std::holds_alternative<MissileAttackState*>(nowState)) {
			boss_->StateManager()->ChangeRequest(std::make_unique<MissileAttackState>());
		}
		// 上下状態
		else if (std::holds_alternative<UpDownState*>(nowState)) {
			boss_->StateManager()->ChangeRequest(std::make_unique<MoveState>());
		}
	}
}

void BossState::StateDecider::DefaultLoop(StateVariant nowState)
{
	// 移動状態
	if (std::holds_alternative<MoveState*>(nowState)) {
		boss_->StateManager()->ChangeRequest(std::make_unique<UpDownState>());
	}
	// 攻撃状態
	else if (std::holds_alternative<MissileAttackState*>(nowState)) {
		boss_->StateManager()->ChangeRequest(std::make_unique<MoveState>());
		MoveState* newState = static_cast<MoveState*>(boss_->GetState());
		newState->MoveSelect(player_->worldTransform_.GetWorldPosition());
		newState->TestProcess();
	}
	// 上下状態
	else if (std::holds_alternative<UpDownState*>(nowState)) {
		boss_->StateManager()->ChangeRequest(std::make_unique<MissileAttackState>());
	}
}

void BossState::StateDecider::NearLoop(StateVariant nowState)
{
	// 移動状態
	if (std::holds_alternative<MoveState*>(nowState)) {
		boss_->StateManager()->ChangeRequest(std::make_unique<UpDownState>());
	}
	// 攻撃状態
	else if (std::holds_alternative<MissileAttackState*>(nowState)) {
		boss_->StateManager()->ChangeRequest(std::make_unique<MoveState>());
		MoveState* newState = static_cast<MoveState*>(boss_->GetState());
		newState->MoveSelect(player_->worldTransform_.GetWorldPosition());
		newState->TestProcess();
	}
	// 上下状態
	else if (std::holds_alternative<UpDownState*>(nowState)) {
		boss_->StateManager()->ChangeRequest(std::make_unique<MoveState>());
		MoveState* newState = static_cast<MoveState*>(boss_->GetState());
		newState->MoveSelect(player_->worldTransform_.GetWorldPosition());
		newState->TestProcess();
	}
}

void BossState::IState::PreInitialize(Boss* boss)
{
	// ポインタ設定
	boss_ = boss;

}

void BossState::IState::TimerUpdate(StateVariant state)
{
	// 変更タイマー
	changeTimer_.Update();
	// 終了時に変更
	if (changeTimer_.IsEnd()) {
		boss_->GetDecider().StateDecide(state);
		return;
	}
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
		float scale = 0.4f;
		pos.scale = { scale,scale,scale };
		Vector3 moveDirect = Vector3::Normalize(boss_->GetPlayer()->worldTransform_.GetWorldPosition() - boss_->worldTransform_.GetWorldPosition());
		boss_->GetBulletManager()->GetBeginCluster()->AddBullet(pos, moveDirect,50.0f);
		fireTimer_.Start(10.0f);
	}
	// 射撃タイマー
	fireTimer_.Update();

	TimerUpdate(this);
}

void BossState::MissileAttackState::Exit()
{

}

void BossState::MoveState::Initialize()
{
	changeTimer_.Start(150.0f);
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
	TimerUpdate(this);
}

void BossState::MoveState::Exit()
{
	isLeft_ = false;
}

void BossState::MoveState::MoveSelect(const Vector3& playerPosition)
{
	// 右前
	if (playerPosition.x > 0 && playerPosition.z > 0) {

	}
	// 左前
	else if(playerPosition.x <= 0 && playerPosition.z > 0){

	}
	// 右後ろ
	else if (playerPosition.x > 0 && playerPosition.z <= 0) {

	}
	// 左後ろ
	else if (playerPosition.x <= 0 && playerPosition.z <= 0) {

	}
}

void BossState::MoveState::TestProcess()
{
	//changeTimer_.Start(300.0f);
	isLeft_ = true;
}

void BossState::UpDownState::Initialize()
{
	float offset = 15.0f;
	if (boss_->worldTransform_.GetWorldPosition().y > 20.0f) {
		startPosition_ = boss_->worldTransform_.GetWorldPosition();
		endPosition_ = startPosition_;
		endPosition_.y -= offset;
	}
	else {
		startPosition_ = boss_->worldTransform_.GetWorldPosition();
		endPosition_ = startPosition_;
		endPosition_.y += offset;
	}
	// 変更のタイマー
	changeTimer_.Start(120.0f);

}

void BossState::UpDownState::Update()
{
	// 移動処理
	boss_->worldTransform_.transform_.translate = Vector3::Lerp(startPosition_, endPosition_, changeTimer_.GetElapsedFrame());
	TimerUpdate(this);
}

void BossState::UpDownState::Exit()
{

}

void BossState::WaitState::Initialize()
{

}

void BossState::WaitState::Update()
{
	TimerUpdate(this);
}

void BossState::WaitState::Exit()
{

}
