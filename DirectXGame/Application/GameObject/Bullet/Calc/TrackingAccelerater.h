#pragma once
#include "../BulletEnums.h"

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
	Vector3 CalcTrackingAcceleration(const Vector3& toDirect);

private:
	// 弾本体
	TrackingBullet* bullet_ = nullptr;
	// 追従データ
	TrackingData data_ = {};
};
