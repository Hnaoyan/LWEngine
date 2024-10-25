#pragma once
#include "../ITrackingState.h"

class TrackingWaveringState : public ITrackingState
{
public:
	void Enter() override;
	void Update() override;
	void Exit() override;

};
