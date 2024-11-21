#pragma once
#include "../ITrackingState.h"
#include <functional>

/// <summary>
/// 追従の波挙動状態
/// </summary>
class TrackingWaveringState : public ITrackingState
{
public:
	void Enter() override;
	void Update(BulletStateMachine& stateMachine) override;
	void Exit() override;

private:
	// 三角関数での計算Ｖｅｒ
	void TrigonometricPattern();
	// 右
	void RightCurve();
	// 左
	void LeftCurve();
	// 終了更新
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
	// カーブの関数ポインタ
	std::function<void(void)> curveProcess_;

};
