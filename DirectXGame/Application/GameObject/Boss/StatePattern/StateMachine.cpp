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
	tables_["First"] = {};
	tables_["First"].patterns.push_back(StatePattern::kMove);
	tables_["First"].patterns.push_back(StatePattern::kUpdown);
	tables_["First"].patterns.push_back(StatePattern::kAttack);
	tables_["First"].patterns.push_back(StatePattern::kWait);
	tables_["First"].maxStep = (uint32_t)tables_["First"].patterns.size();
	currentStep_ = 0;
	IsInActionSequence_ = false;
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
	nowState;
	if (!this->IsInActionSequence_) {
		randomValue_ = LwLib::GetRandomValue(0, 10);

		IsInActionSequence_ = true;
		StateSelect(tables_["First"].patterns[currentStep_]);
		currentStep_++;
		return;
	}
	else {
		if (currentStep_ >= tables_["First"].maxStep) {
			IsInActionSequence_ = false;
			currentStep_ = 0;
			return;
		}
		StateSelect(tables_["First"].patterns[currentStep_]);
		currentStep_++;
		return;
	}

	//// ステートが変わる部分（ここで変更先の分岐
	//if (boss_->GetPrevState()) {
	//	DefaultLoop(nowState);
	//}
	//else {
	//	// 移動状態
	//	if (std::holds_alternative<MoveState*>(nowState)) {
	//		boss_->StateManager()->ChangeRequest(std::make_unique<UpDownState>());
	//	}
	//	// 攻撃状態
	//	else if (std::holds_alternative<AttackState*>(nowState)) {
	//		boss_->StateManager()->ChangeRequest(std::make_unique<AttackState>());
	//	}
	//	// 上下状態
	//	else if (std::holds_alternative<UpDownState*>(nowState)) {
	//		boss_->StateManager()->ChangeRequest(std::make_unique<MoveState>());
	//	}
	//	else {
	//		boss_->StateManager()->ChangeRequest(std::make_unique<WaitState>());
	//	}
	//}
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

void BossState::StateDecider::StateSelect(StatePattern number)
{
	switch (number)
	{
	case BossState::StateDecider::StatePattern::kAttack:
		boss_->StateManager()->ChangeRequest(std::make_unique<AttackState>());
		break;
	case BossState::StateDecider::StatePattern::kMove:
		boss_->StateManager()->ChangeRequest(std::make_unique<MoveState>());
		break;
	case BossState::StateDecider::StatePattern::kUpdown:
		boss_->StateManager()->ChangeRequest(std::make_unique<UpDownState>());
		break;
	case BossState::StateDecider::StatePattern::kWait:
		boss_->StateManager()->ChangeRequest(std::make_unique<WaitState>());
		break;
	case BossState::StateDecider::StatePattern::kTeleport:
		boss_->StateManager()->ChangeRequest(std::make_unique<TeleportState>());
		break;
	case BossState::StateDecider::StatePattern::kMax:
		break;
	default:
		break;
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
	if (changeTimer_.IsEnd() || !changeTimer_.IsActive()) {
		boss_->GetDecider()->StateDecide(state);
		return;
	}
}
