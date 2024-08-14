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
	//float distance = Vector3::Distance(boss_->worldTransform_.GetWorldPosition(), player_->worldTransform_.GetWorldPosition());

	//if (distance >= 100.0f) {
	//	boss_->StateManager()->ChangeRequest(std::make_unique<MoveState>());
	//	MoveState* newState = static_cast<MoveState*>(boss_->GetState());
	//	newState->MoveSelect(player_->worldTransform_.GetWorldPosition());
	//	newState->TestProcess();
	//	return;
	//}

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
		else if (std::holds_alternative<AttackState*>(nowState)) {
			boss_->StateManager()->ChangeRequest(std::make_unique<AttackState>());
		}
		// 上下状態
		else if (std::holds_alternative<UpDownState*>(nowState)) {
			boss_->StateManager()->ChangeRequest(std::make_unique<MoveState>());
		}
		else {
			boss_->StateManager()->ChangeRequest(std::make_unique<WaitState>());
		}
	}
}

void BossState::StateDecider::DefaultLoop(StateVariant nowState)
{
	// 移動状態
	if (std::holds_alternative<MoveState*>(nowState)) {
		boss_->StateManager()->ChangeRequest(std::make_unique<TeleportState>());
		TeleportState* newState = static_cast<TeleportState*>(boss_->GetState());
		newState->SelectTeleportPoint(player_->worldTransform_.GetWorldPosition());
	}
	// 攻撃状態
	else if (std::holds_alternative<AttackState*>(nowState)) {
		if (std::holds_alternative<TeleportState*>(boss_->prevVariantState_)) {
			boss_->StateManager()->ChangeRequest(std::make_unique<UpDownState>());
		}
		else {
			boss_->StateManager()->ChangeRequest(std::make_unique<MoveState>());
			MoveState* newState = static_cast<MoveState*>(boss_->GetState());
			//newState->MoveSelect(player_->worldTransform_.GetWorldPosition());
			newState->TestProcess();
		}
	}
	// 上下状態
	else if (std::holds_alternative<UpDownState*>(nowState)) {
		if (std::holds_alternative<AttackState*>(boss_->prevVariantState_)) {
			boss_->StateManager()->ChangeRequest(std::make_unique<WaitState>());
			return;
		}
		boss_->StateManager()->ChangeRequest(std::make_unique<AttackState>());
	}
	// 待機状態
	else if (std::holds_alternative<WaitState*>(nowState)) {
		boss_->StateManager()->ChangeRequest(std::make_unique<MoveState>());
		MoveState* newState = static_cast<MoveState*>(boss_->GetState());
		//newState->MoveSelect(player_->worldTransform_.GetWorldPosition());
		newState->TestProcess();
	}
	// 瞬間移動状態
	else if (std::holds_alternative<TeleportState*>(nowState)) {
		boss_->StateManager()->ChangeRequest(std::make_unique<AttackState>());
	}
}

void BossState::StateDecider::NearLoop(StateVariant nowState)
{
	// 移動状態
	if (std::holds_alternative<MoveState*>(nowState)) {
		boss_->StateManager()->ChangeRequest(std::make_unique<UpDownState>());
	}
	// 攻撃状態
	else if (std::holds_alternative<AttackState*>(nowState)) {
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
