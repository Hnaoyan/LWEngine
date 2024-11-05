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

	if (Input::GetInstance()->TriggerKey(DIK_O)) {
		stateMachine.RequestState(TrackingState::kWave);
		//stateMachine.ChangeRequest(TrackingState::kWave);
	}
}

void TrackingStraightState::Exit()
{

}
