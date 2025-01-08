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
	/// 追従の計算
	/// </summary>
	/// <param name="toDirect"></param>
	/// <param name="timer"></param>
	/// <returns></returns>
	Vector3 CalcTrackingAcceleration(const Vector3& toDirect, FrameTimer& timer);

	//Vector3 CalcTrackingAcceleration(const Vector3& toDirect, FrameTimer& timer);

private:
	// 弾本体
	TrackingBullet* bullet_ = nullptr;
	// 追従データ
	TrackingData data_ = {};
	// 前フレームの加速度
	Vector3 prevAccelerate_ = {};

};
