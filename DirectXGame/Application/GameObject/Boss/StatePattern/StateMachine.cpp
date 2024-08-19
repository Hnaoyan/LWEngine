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

	tableTag_ = "Default";
	tables_[tableTag_].patterns.push_back(StatePattern::kMissile);
	tables_[tableTag_].patterns.push_back(StatePattern::kMove);
	tables_[tableTag_].patterns.push_back(StatePattern::kUpdown);
	tables_[tableTag_].patterns.push_back(StatePattern::kAttack);
	tables_[tableTag_].patterns.push_back(StatePattern::kWait);
	tables_[tableTag_].patterns.push_back(StatePattern::kUpdown);
	tables_[tableTag_].maxStep = (uint32_t)tables_[tableTag_].patterns.size() - 1;

	tableTag_ = "MoveType";
	tables_[tableTag_].patterns.push_back(StatePattern::kMissile);
	tables_[tableTag_].patterns.push_back(StatePattern::kUpdown);
	tables_[tableTag_].patterns.push_back(StatePattern::kMove);
	tables_[tableTag_].patterns.push_back(StatePattern::kUpdown);
	tables_[tableTag_].patterns.push_back(StatePattern::kMove);
	tables_[tableTag_].patterns.push_back(StatePattern::kWait);
	tables_[tableTag_].maxStep = (uint32_t)tables_[tableTag_].patterns.size() - 1;

	tableTag_ = "AttackType";
	tables_[tableTag_].patterns.push_back(StatePattern::kMissile);
	//tables_[tableTag_].patterns.push_back(StatePattern::kAttack);
	tables_[tableTag_].patterns.push_back(StatePattern::kWait);
	tables_[tableTag_].patterns.push_back(StatePattern::kMissile);
	tables_[tableTag_].maxStep = (uint32_t)tables_[tableTag_].patterns.size() - 1;

	currentStep_ = 0;
	IsInActionSequence_ = false;
	isCooltime_ = false;
}

void BossState::StateDecider::StateDecide(StateVariant nowState)
{
	nowState;
	if (isCooltime_) {
		boss_->StateManager()->ChangeRequest(std::make_unique<WaitState>());
		WaitState* newState = static_cast<WaitState*>(boss_->GetState());
		newState->SetTimer(240.0f);
		isCooltime_ = false;
		return;
	}

	if (!this->IsInActionSequence_) {
		randomValue_ = LwLib::GetRandomValue(0, 6);
		float rand = LwLib::GetRandomValue(0.0f, 6.0f);
		rand;
		RandomTable(2);

		IsInActionSequence_ = true;
		StateSelect(tables_[tableTag_].patterns[currentStep_]);
		currentStep_++;
		return;
	}
	else {
		if (currentStep_ > tables_[tableTag_].maxStep) {
			isCooltime_ = true;
			IsInActionSequence_ = false;
			currentStep_ = 0;
			return;
		}
		StateSelect(tables_[tableTag_].patterns[currentStep_]);
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
	case BossState::StateDecider::StatePattern::kMissile:
		boss_->StateManager()->ChangeRequest(std::make_unique<MissileAttackState>());
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
