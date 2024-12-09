#pragma once
#include "../ITrackingState.h"

/// <summary>
/// 追従移動状態
/// </summary>
class TrackingMoveState : public ITrackingState
{
public:
	void Enter() override;
	void Update(BulletStateMachine& stateMachine) override;
	void Exit() override;
private:
	// 劣等型用のオフセット
	Vector3 inferiorOffset_{};
	// 親となる加速度
	Vector3 parentAcceleration_{};
	// 子となる加速度
	Vector3 childAcceleration_{};

	FrameTimer looseTimer_;	// 追従解除が緩くなるまでのタイマー

private: // 計算関数
	// 優等（完全追尾
	Vector3 CalcSuperiorAcceleration();
	// 劣等（ずれ追尾
	Vector3 CalcInferiorAcceleration();
	// 秀才（先読み追尾
	Vector3 CalcGeniusAcceleration();
};
