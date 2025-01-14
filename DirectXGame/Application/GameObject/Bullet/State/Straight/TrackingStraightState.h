#pragma once
#include "../ITrackingState.h"

/// <summary>
/// 追従の直進状態
/// </summary>
class TrackingStraightState : public ITrackingState
{
public:
	void Enter() override;
	void Update(BulletStateMachine& stateMachine) override;
	void Exit() override;

};