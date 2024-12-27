#include "TrackingAccelerater.h"
#include "../BulletsPaths.h"
#include "Application/GameObject/IGameObject.h"

Vector3 TrackingAccelerater::CalcSuperiorAcceleration()
{
	float speed = bullet_->GetTrackingData().baseSpeed;
	//float maxSpeed = bullet_->GetTrackingData().baseSpeed + 250.0f;
	//if (accelerationTime_.IsActive()) {
	//	speed = Ease::Easing(speed, maxSpeed, accelerationTime_.GetElapsedFrame());
	//}
	//else {
	//	speed = maxSpeed;
	//}

	Vector3 bulletVelocity = bullet_->GetVelocity();

	// それぞれのベクトル
	Vector3 toTarget = bullet_->GetTarget()->worldTransform_.GetWorldPosition() - bullet_->GetWorldPosition();
	Vector3 nowDirect = Vector3::Normalize(bulletVelocity);
	// 内積
	float dot = Vector3::Dot(toTarget, nowDirect);
	// 向心加速力の計算
	Vector3 centripetalAccel = toTarget - (nowDirect * dot);
	float centripetalAccelMagnitude = Vector3::Length(centripetalAccel);
	// 大きさの調整
	// 最大値
	float maxCentripetalAccel = 1.0f;
	if (centripetalAccelMagnitude > maxCentripetalAccel) {
		centripetalAccel /= centripetalAccelMagnitude;
	}
	// 最大向心力
	float maxCentripetalForce = std::powf(speed, 2) / bullet_->GetTrackingData().lerpRadius;

	// 力の向き
	Vector3 force = centripetalAccel * maxCentripetalForce;
	// 推進力計算
	float propulsion = speed * bullet_->GetTrackingData().damping;
	// 向心力に現在の方向ベクトルに＋推進力でベクトルを作成
	force += nowDirect * propulsion;
	// 速度の減衰処理
	force -= bulletVelocity * bullet_->GetTrackingData().damping;

	// 加速度の計算
	return Vector3(force);
}

Vector3 TrackingAccelerater::CalcInferiorAcceleration(const Vector3& offset)
{
	Vector3 bulletVelocity = bullet_->GetVelocity();
	// それぞれのベクトル
	Vector3 targetPoint = bullet_->GetTarget()->worldTransform_.GetWorldPosition() + offset;
	Vector3 toTarget = targetPoint - bullet_->GetWorldPosition();
	Vector3 nowDirect = Vector3::Normalize(bulletVelocity);
	// 内積
	float dot = Vector3::Dot(toTarget, nowDirect);
	// 向心加速力の計算
	Vector3 centripetalAccel = toTarget - (nowDirect * dot);
	float centripetalAccelMagnitude = Vector3::Length(centripetalAccel);
	// 大きさの調整
	// 最大値
	float maxCentripetalAccel = 2.0f;
	if (centripetalAccelMagnitude > maxCentripetalAccel) {
		centripetalAccel /= centripetalAccelMagnitude;
	}
	// 最大向心力
	float maxCentripetalForce = std::powf(bullet_->GetTrackingData().baseSpeed, 2) / bullet_->GetTrackingData().lerpRadius;
	// 力の向き
	Vector3 force = centripetalAccel * maxCentripetalForce;
	// 推進力計算
	float propulsion = bullet_->GetTrackingData().baseSpeed * bullet_->GetTrackingData().damping;
	// 向心力に現在の方向ベクトルに＋推進力でベクトルを作成
	force += nowDirect * propulsion;
	// 速度の減衰処理
	force -= bulletVelocity * bullet_->GetTrackingData().damping;

	return Vector3(force);
}

Vector3 TrackingAccelerater::CalcGeniusAcceleration()
{
	Vector3 bulletVelocity = bullet_->GetVelocity();
	// プレイヤーの現在の位置と速度
	Vector3 targetPoint = bullet_->GetTarget()->worldTransform_.GetWorldPosition();
	Vector3 playerDirection = bullet_->GetTarget()->worldTransform_.GetWorldPosition() - bullet_->GetTarget()->prevPosition_;
	Vector3 predictedPosition{};	// 予測先の座標
	// 予測位置を計算
	float predictionTime = GlobalVariables::GetInstance()->GetValue<float>("TrackSuperior", "PredictionTime"); // ミサイルが向かう予測時間
	// 予測先の計算
	if (playerDirection.x == 0.0f && playerDirection.y == 0.0f && playerDirection.z == 0.0f) {
		predictedPosition = targetPoint;
	}
	else {
		predictedPosition = targetPoint + (playerDirection * predictionTime);
	}

	Vector3 toTarget = predictedPosition - bullet_->GetWorldPosition();
	Vector3 nowDirect = Vector3::Normalize(bulletVelocity);
	float dot = Vector3::Dot(toTarget, nowDirect);

	Vector3 centripetalAccel = toTarget - (nowDirect * dot);
	float centripetalAccelMagnitude = Vector3::Length(centripetalAccel);

	// 最大値
	float maxCentripetalAccel = 2.0f;
	if (centripetalAccelMagnitude > maxCentripetalAccel) {
		centripetalAccel /= centripetalAccelMagnitude;
	}
	// 最大向心力
	float maxCentripetalForce = std::powf(bullet_->GetTrackingData().baseSpeed, 2) / bullet_->GetTrackingData().lerpRadius;
	// 力の向き
	Vector3 force = centripetalAccel * maxCentripetalForce;
	// 推進力計算
	float propulsion = bullet_->GetTrackingData().baseSpeed * bullet_->GetTrackingData().damping;
	// 向心力に現在の方向ベクトルに＋推進力でベクトルを作成
	force += nowDirect * propulsion;
	force -= bulletVelocity * bullet_->GetTrackingData().damping;

	return Vector3(force);
}

Vector3 TrackingAccelerater::CalcTrackingAcceleration(const Vector3& toDirect, FrameTimer& timer)
{
	// 速度
	float speed = bullet_->GetTrackingData().baseSpeed;
	// オフセット
	float maxOffset = 100.0f;
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
	float trackingAccuracy = GlobalVariables::GetInstance()->GetValue<float>("TrackingData","TrackingAccuracy");

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
	float maxCentripetalAccel = 2.0f;
	if (centripetalAccelMagnitude > maxCentripetalAccel) {
		centripetalAccel /= centripetalAccelMagnitude;
	}
	// 最大向心力
	float maxCentripetalForce = std::powf(speed, 2) / bullet_->GetTrackingData().lerpRadius;

	// 最大向心力に命中率を適用（命中率が低いほど追従力を弱くする）
	maxCentripetalForce *= trackingAccuracy;
	// 力の向き
	Vector3 force = centripetalAccel * maxCentripetalForce;

	// 推進力計算
	float propulsion = speed * bullet_->GetTrackingData().damping;

	// 向心力に現在の方向ベクトルに＋推進力でベクトルを作成
	force += nowDirect * propulsion;
	force -= bulletVelocity * bullet_->GetTrackingData().damping;

	// 目標方向から完全に一致しないように、少しだけ力を調整
	// 弾がターゲット方向に近づきつつも、完全には収束しないように
	force += (Vector3::Normalize(toDirect - bulletVelocity) - nowDirect) * (1.0f - trackingAccuracy);

	return Vector3(force);
}
