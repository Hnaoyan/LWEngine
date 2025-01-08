#include "TrackingAccelerater.h"
#include "../BulletsPaths.h"
#include "Application/GameObject/IGameObject.h"
#include "Application/GameObject/Bullet/BulletManager.h"
#include "Application/GameObject/Bullet/BulletEnums.h"

Vector3 TrackingAccelerater::CalcTrackingAcceleration(const Vector3& toDirect, FrameTimer& timer)
{
	AccuracyData accuracy = {};
	if (bullet_->GetTrackingData().accuracyType == 0) {
		accuracy.forceDamping = bullet_->GetTrackingData().damping;
		accuracy.propulsionDamping = bullet_->GetTrackingData().damping;
		accuracy.smoothFactor = 0.0f;
		accuracy.trackingAccuracy = GlobalVariables::GetInstance()->GetValue<float>("TrackingData", "TrackingAccuracy");
		accuracy.trackingAccuracy = 1.0f;
	}

	// 速度
	float speed = bullet_->GetTrackingData().baseSpeed;
	// オフセット
	float maxOffset = BulletManager::sSpeedLimitOffset;
	// 最大速度
	float maxSpeed = bullet_->GetTrackingData().baseSpeed + maxOffset;
	// 調整
	if (timer.IsActive()) {
		speed = Ease::Easing(speed, maxSpeed, timer.GetElapsedFrame());
	}
	else {
		speed = maxSpeed;
	}
	// 命中率の割合変数（0.0 = 完全に外れる, 1.0 = 完全に命中）
	float trackingAccuracy = accuracy.trackingAccuracy;

	// 弾の速度
	Vector3 bulletVelocity = bullet_->GetVelocity();
	// 方向
	Vector3 nowDirect = Vector3::Normalize(bulletVelocity);
	// 方向内積
	float dot = Vector3::Dot(toDirect, nowDirect);
	// 向心加速力の計算
	Vector3 centripetalAccel = toDirect - (nowDirect * dot);
	float centripetalAccelMagnitude = Vector3::Length(centripetalAccel);

	// 大きさの調整
	// 最大値
	float maxCentripetalAccel = BulletManager::sTrackingMaxCentripetal;
	if (centripetalAccelMagnitude > maxCentripetalAccel) {
		centripetalAccel /= centripetalAccelMagnitude;
	}
	// 最大向心力
	float maxCentripetalForce = std::min(std::powf(speed, 2) / bullet_->GetTrackingData().lerpRadius, BulletManager::sMaxCentripetalForce);

	// 最大向心力に命中率を適用（命中率が低いほど追従力を弱くする）
	maxCentripetalForce *= trackingAccuracy;
	// 力の向き
	Vector3 force = centripetalAccel * maxCentripetalForce;

	// 推進力計算
	float propulsion = speed * accuracy.propulsionDamping;

	// 向心力に現在の方向ベクトルに＋推進力でベクトルを作成
	force += nowDirect * propulsion;
	force -= bulletVelocity * accuracy.forceDamping;	// ここの減衰率を推進力と一致させないとずれる

	// 目標方向から完全に一致しないように、少しだけ力を調整
	// 弾がターゲット方向に近づきつつも、完全には収束しないように
	force += (Vector3::Normalize(toDirect - bulletVelocity) - nowDirect) * (1.0f - trackingAccuracy);

	// 平滑化
	float smoothFactor = accuracy.smoothFactor;
	force = Ease::Easing(force, prevAccelerate_, smoothFactor);
	// 前フレ加速度保存
	prevAccelerate_ = force;

	return Vector3(force);
}
