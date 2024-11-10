#include "TrackingStraightState.h"
#include "../../../BulletsLists.h"
#include "../StateMachine/BulletStateMachine.h"
#include "Engine/Input/Input.h"

void TrackingStraightState::Enter()
{
	bullet_->SetAccelerate(Vector3::Zero());
}

void TrackingStraightState::Update(BulletStateMachine& stateMachine)
{
	timer_.Update();

	bullet_->SetAccelerate(bullet_->GetVelocity() * 1.0f / 300.0f);

	//if (timer_.IsEnd()) {
	//	stateMachine.RequestState(TrackingState::kWave);
	//}

	if (Input::GetInstance()->TriggerKey(DIK_O)) {
		stateMachine.RequestState(TrackingState::kWave);
		//stateMachine.ChangeRequest(TrackingState::kWave);
	}
}

void TrackingStraightState::Exit()
{

}
