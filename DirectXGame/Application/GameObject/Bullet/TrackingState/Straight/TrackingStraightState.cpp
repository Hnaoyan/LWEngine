#include "TrackingStraightState.h"
#include "../../BulletsPaths.h"
#include "../StateMachine/BulletStateMachine.h"
#include "Engine/Input/Input.h"

void TrackingStraightState::Enter()
{
	bullet_->SetAccelerate(Vector3::Zero());
}

void TrackingStraightState::Update(BulletStateMachine& stateMachine)
{
	timer_.Update();

	float damping = 1.0f / 300.0f;
	// 速度を減衰させた値を設定
	bullet_->SetAccelerate(bullet_->GetVelocity() * damping);

#ifdef IMGUI_ENABLED
	if (Input::GetInstance()->TriggerKey(DIK_O)) {
		stateMachine.RequestState(TrackingState::kTurnToTarget);
	}
#endif // IMGUI_ENABLED
}

void TrackingStraightState::Exit()
{

}
