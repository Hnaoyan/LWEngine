#pragma once
#include "../Core/ICamera.h"
#include "../../3D/WorldTransform.h"

namespace PlayerContext {
	class LockOn;
}

/// <summary>
/// 追従カメラ
/// </summary>
class FollowCamera : public ICamera
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;
	/// <summary>
	/// ImGui
	/// </summary>
	void ImGuiDraw() override;

	/// <summary>
	/// 
	/// </summary>
	void InitializeGlobalValue() override;
public:
	/// <summary>
	/// リセット
	/// </summary>
	void Reset();
	/// <summary>
	/// オフセット作成
	/// </summary>
	/// <returns></returns>
	Vector3 CreateOffset() const;

public:
	/// <summary>
	/// 対象の設定
	/// </summary>
	/// <param name="parent"></param>
	void SetParent(const WorldTransform* target) {
		target_ = target;
		Reset();
	}

	void SetLockOn(const PlayerContext::LockOn* lockOn) {
		lockOn_ = lockOn;
	}

private: // UserMethod
	// ターゲット
	const WorldTransform* target_ = nullptr;

	const PlayerContext::LockOn* lockOn_ = nullptr;

	// デフォルトオフセット
	Vector3 defaultOffset_ = {};
	// 残像座標
	Vector3 interTarget_ = {};
	// 目標地点の角度
	Vector3 destinationAngle_ = {};
	float delayRate_ = 0.01f;

	// 右スティックのカメラ操作関係
	float rStickRotateSpeed_ = 0;
	float rStickLerpRate_ = 0;
	Vector2 rRotateSpeed_{};

	float xSpinLimit_ = 0.0f;

	bool isAtan_ = false;

	struct ShiftData
	{
		FrameTimer actionTimer;	// タイマー
		float shiftDistance;	// シフトする距離

	};
	// ダッシュエフェクト用
	ShiftData dashEffect_;

	// ロックオンに対してスムーズに移行するように
	FrameTimer lockSmoothTimer_;

};
