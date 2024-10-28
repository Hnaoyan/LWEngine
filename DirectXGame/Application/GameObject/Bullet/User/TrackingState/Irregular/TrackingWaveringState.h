#pragma once
#include "../ITrackingState.h"

class TrackingWaveringState : public ITrackingState
{
public:
	void Enter() override;
	void Update(BulletStateMachine& stateMachine) override;
	void Exit() override;

private:
	void TrigonometricPattern();

private:
	// 揺らす基準のベクトル
	Vector3 defaultDirect_{};
	float waveCount_ = 0.0f;

	// 正なら右 負なら左
	int32_t curveDirect_ = 1;

	// 曲がる時間
	FrameTimer curveTimer_;

};
