#pragma once
#include "../ITrackingState.h"

class TrackingStraightState : public ITrackingState
{
public:
	void Enter() override;
	void Update(BulletStateMachine& stateMachine) override;
	void Exit() override;

};