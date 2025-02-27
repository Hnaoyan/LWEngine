#include "BulletStateMachine.h"
#include "../TrackingStates.h"

BulletStateMachine::BulletStateMachine(IBullet* bullet)
{
	bullet_ = bullet;
	currentState_ = std::make_unique<TrackingStraightState>();
}

BulletStateMachine::BulletStateMachine()
{
	currentState_ = std::make_unique<TrackingStraightState>();
}

void BulletStateMachine::ChangeRequest(std::unique_ptr<ITrackingState> state)
{
	// 終了時の関数
	if (currentState_) {
		currentState_->Exit();
	}

	changeCount_++;

	state->SetBullet(bullet_);
	state->Enter();
	currentState_ = std::move(state);
}

void BulletStateMachine::ChangeRequest(TrackingState state)
{
	if (currentState_) {
		currentState_->Exit();
	}
	std::unique_ptr<ITrackingState> newState = BuildState(state);

	changeCount_++;

	newState->SetBullet(bullet_);
	newState->Enter();
	currentState_ = std::move(newState);
}

std::unique_ptr<ITrackingState> BulletStateMachine::BuildState(TrackingState newState)
{
	std::unique_ptr<ITrackingState> instance;

	switch (newState)
	{
	case TrackingState::kStraight: // 直進
		instance = std::make_unique<TrackingStraightState>();
		break;
	case TrackingState::kWave: // 波の不規則挙動
		instance = std::make_unique<TrackingWaveringState>();
		break;
	case TrackingState::kTracking: // 追尾
		instance = std::make_unique<TrackingMoveState>();
		break;
	case TrackingState::kTurnToTarget:
		instance = std::make_unique<TurnToTargetState>();
		break;
	}

	return std::move(instance);
}


void BulletStateMachine::Update(bool isActive)
{
	//// 切り替えの受付
	//if (currentState_->GetChangeRequest()) {
	//	TrackingState newState = currentState_->GetChangeRequest().value();
	//	ChangeRequest(newState);
	//	currentState_->RequestReset();
	//}
	// フラグで更新の選択を
	if (isActive) {
		// ステートの処理
		currentState_->Update(*this);
	}
}
