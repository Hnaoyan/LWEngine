#pragma once
#include "../ITrackingState.h"

class TrackingWaveringState : public ITrackingState
{
public:
	void Enter() override;
	void Update(BulletStateMachine& stateMachine) override;
	void Exit() override;

private:
	// 揺らす基準のベクトル
	Vector3 defaultDirect_{};

};
