#include "TrackingWaveringState.h"
#include "../../../BulletsLists.h"
#include "../StateMachine/BulletStateMachine.h"

void TrackingWaveringState::Enter()
{
	// 基準設定
	defaultDirect_ = Vector3::Normalize(bullet_->GetVelocity());
	bullet_->SetAccelerate(Vector3::Zero());
}

void TrackingWaveringState::Update(BulletStateMachine& stateMachine)
{
	stateMachine;
}

void TrackingWaveringState::Exit()
{

}
