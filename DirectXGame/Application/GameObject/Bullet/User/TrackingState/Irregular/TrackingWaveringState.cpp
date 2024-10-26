#include "TrackingWaveringState.h"
#include "../../../BulletsLists.h"
#include "../StateMachine/BulletStateMachine.h"

void TrackingWaveringState::Enter()
{
	// 基準設定
	defaultDirect_ = Vector3::Normalize(bullet_->GetVelocity());
}

void TrackingWaveringState::Update(BulletStateMachine& stateMachine)
{
	stateMachine;
}

void TrackingWaveringState::Exit()
{

}
