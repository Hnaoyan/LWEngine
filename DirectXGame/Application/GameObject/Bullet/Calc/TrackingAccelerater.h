#pragma once
#include "../BulletEnums.h"
#include "Engine/LwLib/Utillity/FrameTimer.h"

class TrackingBullet;

class TrackingAccelerater
{
public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	~TrackingAccelerater() = default;
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="bullet"></param>
	TrackingAccelerater(TrackingBullet* bullet) : bullet_(bullet) {}
	TrackingAccelerater() {};
public:
	/// <summary>
	/// 優等型
	/// </summary>
	/// <returns></returns>
	Vector3 CalcSuperiorAcceleration();
	/// <summary>
	/// 劣等型
	/// </summary>
	/// <param name="offset"></param>
	/// <returns></returns>
	Vector3 CalcInferiorAcceleration(const Vector3& offset);
	/// <summary>
	/// 秀才型
	/// </summary>
	/// <returns></returns>
	Vector3 CalcGeniusAcceleration();

public:
	/// <summary>
	/// 追従の計算
	/// </summary>
	/// <param name="toDirect"></param>
	/// <param name="timer"></param>
	/// <returns></returns>
	Vector3 CalcTrackingAcceleration(const Vector3& toDirect, FrameTimer& timer);

private:
	// 弾本体
	TrackingBullet* bullet_ = nullptr;
	// 追従データ
	TrackingData data_ = {};
};
