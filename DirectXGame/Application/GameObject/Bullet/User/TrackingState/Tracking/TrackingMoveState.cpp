#include "TrackingMoveState.h"
#include "Application/GameSystem/GameSystem.h"
#include "Application/GameObject/IGameObject.h"
#include "Engine/GlobalVariables/GlobalVariables.h"

#include "../../../BulletsLists.h"
#include "../StateMachine/BulletStateMachine.h"

void TrackingMoveState::Enter()
{

}

void TrackingMoveState::Update(BulletStateMachine& stateMachine)
{
	// タイマーの更新
	timer_.Update();

	// 変更
	if (timer_.IsEnd()) {
		stateMachine.ChangeRequest(TrackingState::kWave);
	}
}

void TrackingMoveState::Exit()
{

}

Vector3 TrackingMoveState::CalcSuperiorAcceleration()
{
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
	if (centripetalAccelMagnitude > 2.0f) {
		centripetalAccel /= centripetalAccelMagnitude;
	}
	// 最大向心力
	float maxCentripetalForce = std::powf(TrackingBullet::sBaseVelocity, 2) / TrackingBullet::sLerpRadius;

	// 力の向き
	Vector3 force = centripetalAccel * maxCentripetalForce;
	// 推進力計算
	float propulsion = TrackingBullet::sBaseVelocity * TrackingBullet::sDamping;
	// 向心力に現在の方向ベクトルに＋推進力でベクトルを作成
	force += nowDirect * propulsion;
	// 速度の減衰処理
	force -= bulletVelocity * TrackingBullet::sDamping;

	// 加速度の計算
	return Vector3(force * GameSystem::GameSpeedFactor());
}

Vector3 TrackingMoveState::CalcInferiorAcceleration()
{
	Vector3 offset = {};
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
	if (centripetalAccelMagnitude > 2.0f) {
		centripetalAccel /= centripetalAccelMagnitude;
	}
	// 最大向心力
	float maxCentripetalForce = std::powf(TrackingBullet::sBaseVelocity, 2) / TrackingBullet::sLerpRadius;
	// 力の向き
	Vector3 force = centripetalAccel * maxCentripetalForce;
	// 推進力計算
	float propulsion = TrackingBullet::sBaseVelocity * TrackingBullet::sDamping;
	// 向心力に現在の方向ベクトルに＋推進力でベクトルを作成
	force += nowDirect * propulsion;
	// 速度の減衰処理
	force -= bulletVelocity * TrackingBullet::sDamping;

	return Vector3(force * GameSystem::GameSpeedFactor());
}

Vector3 TrackingMoveState::CalcGeniusAcceleration()
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

	if (centripetalAccelMagnitude > 2.0f) {
		centripetalAccel /= centripetalAccelMagnitude;
	}

	float maxCentripetalForce = std::powf(TrackingBullet::sBaseVelocity, 2) / TrackingBullet::sLerpRadius;

	Vector3 force = centripetalAccel * maxCentripetalForce;

	float propulsion = TrackingBullet::sBaseVelocity * TrackingBullet::sDamping;

	force += nowDirect * propulsion;
	force -= bulletVelocity * TrackingBullet::sDamping;

	return Vector3(force * GameSystem::GameSpeedFactor());
}
