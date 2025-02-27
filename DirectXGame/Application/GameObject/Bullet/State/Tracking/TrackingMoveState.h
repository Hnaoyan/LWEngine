#pragma once
#include "../ITrackingState.h"
#include "../../Calc/TrackingAccelerater.h"

/// <summary>
/// 追従移動状態
/// </summary>
class TrackingMoveState : public ITrackingState
{
public:
	void Enter() override;
	void Update(BulletStateMachine& stateMachine) override;
	void Exit() override;

	void CheckPassing();
private:
	/// <summary>
	/// 種類別の更新部分
	/// </summary>
	/// <param name="type"></param>
	void UpdateByType(TrackingAttribute type);

	Vector3 CalculateDirection(TrackingAttribute type);

private:
	// 劣等型用のオフセット
	Vector3 inferiorOffset_{};
	// 親となる加速度
	Vector3 parentAcceleration_{};
	// 子となる加速度
	Vector3 childAcceleration_{};
	// 追従解除が緩くなるまでのタイマー
	FrameTimer looseTimer_;	
	// 加速タイマー
	FrameTimer accelerationTime_;
	//　現在の時間
	float elapsedTime_ = 0.0f;
	// 加速度計算用クラス
	std::unique_ptr<TrackingAccelerater> accelerater_;
	float currentFrame_ = 0.0f;

	int32_t frameCount_ = 0u;
};
