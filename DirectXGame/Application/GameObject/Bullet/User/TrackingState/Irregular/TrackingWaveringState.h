#pragma once
#include "../ITrackingState.h"
#include <functional>

class TrackingWaveringState : public ITrackingState
{
public:
	void Enter() override;
	void Update(BulletStateMachine& stateMachine) override;
	void Exit() override;

private:
	void TrigonometricPattern();
	void RightCurve();
	void LeftCurve();
	void EndUpdate();

private:
	// 揺らす基準のベクトル
	Vector3 defaultDirect_{};
	float waveCount_ = 0.0f;
	float curveFrame_ = 60.0f;

	// 正なら右 負なら左
	int32_t curveDirect_ = 0;

	// 曲がる時間
	FrameTimer curveTimer_;

	std::function<void(void)> curveProcess_;

};
