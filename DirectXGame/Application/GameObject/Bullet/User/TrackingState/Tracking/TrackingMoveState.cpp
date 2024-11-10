#include "TrackingMoveState.h"
#include "Application/GameSystem/GameSystem.h"
#include "Application/GameObject/IGameObject.h"
#include "Engine/GlobalVariables/GlobalVariables.h"

#include "../../../BulletsLists.h"
#include "../StateMachine/BulletStateMachine.h"

void TrackingMoveState::Enter()
{
	GlobalVariables* global = GlobalVariables::GetInstance();
	// 初期化
	inferiorOffset_ = {};
	// ずらすオフセット作成
	std::string groupName = "TrackInferior";
	float offsetValue = global->GetValue<float>(groupName, "MaxOffset");
	// 最小値
	float limit = global->GetValue<float>(groupName, "MinOffset");
	// オフセット
	inferiorOffset_ = LwLib::GetRandomValue({ -offsetValue,-offsetValue,-offsetValue }, { offsetValue,offsetValue,offsetValue }, limit);

	Vector3 newVelocity = bullet_->GetVelocity() + bullet_->GetVelocity() * (1.0f / 300.0f);
	bullet_->SetVelocity(newVelocity);
	//timer_.Start(TrackingBullet::sTrackingFrame);
}

void TrackingMoveState::Update(BulletStateMachine& stateMachine)
{
	// タイマーの更新
	timer_.Update();

	// 変更
	if (timer_.IsEnd()) {
		stateMachine.RequestState(TrackingState::kWave);
		//stateMachine.ChangeRequest(TrackingState::kWave);
	}

	// 誘導弾なら
	if (dynamic_cast<TrackingBullet*>(bullet_)) {

		if (!bullet_->GetTarget()) {
			return;
		}

		float dot = Vector3::Dot(Vector3::Normalize(bullet_->GetVelocity()), Vector3::Normalize(bullet_->GetTarget()->worldTransform_.GetWorldPosition() - bullet_->GetWorldPosition()));
		// 向きが過度に離れていたら追尾しない
		float limitDot = GlobalVariables::GetInstance()->GetValue<float>("BossTrackingBullet", "TrackingDot");
		if (dot < limitDot) {
			return;
		}

		// 種類の受け取り
		TrackingAttribute type = dynamic_cast<TrackingBullet*>(bullet_)->GetTrackingType();
		
		// 種類ごとの計算
		switch (type)
		{
		case TrackingAttribute::kSuperior:
			bullet_->SetAccelerate(CalcSuperiorAcceleration());
			break;
		case TrackingAttribute::kInferior:
			bullet_->SetAccelerate(CalcInferiorAcceleration());
			break;
		case TrackingAttribute::kGenius:
			bullet_->SetAccelerate(CalcGeniusAcceleration());
			break;
		default:
			break;
		}
	}

}

void TrackingMoveState::Exit()
{

}

Vector3 TrackingMoveState::CalcSuperiorAcceleration()
{
	// キャストポインタ
	TrackingBullet* bullet = dynamic_cast<TrackingBullet*>(bullet_);

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
	float maxCentripetalForce = std::powf(bullet->GetTrackingData().baseSpeed, 2) / bullet->GetTrackingData().lerpRadius;

	// 力の向き
	Vector3 force = centripetalAccel * maxCentripetalForce;
	// 推進力計算
	float propulsion = bullet->GetTrackingData().baseSpeed * bullet->GetTrackingData().damping;
	// 向心力に現在の方向ベクトルに＋推進力でベクトルを作成
	force += nowDirect * propulsion;
	// 速度の減衰処理
	force -= bulletVelocity * bullet->GetTrackingData().damping;

	// 加速度の計算
	return Vector3(force * GameSystem::GameSpeedFactor());
}

Vector3 TrackingMoveState::CalcInferiorAcceleration()
{
	// キャストポインタ
	TrackingBullet* bullet = dynamic_cast<TrackingBullet*>(bullet_);

	Vector3 bulletVelocity = bullet_->GetVelocity();
	// それぞれのベクトル
	Vector3 targetPoint = bullet_->GetTarget()->worldTransform_.GetWorldPosition() + inferiorOffset_;
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
	float maxCentripetalForce = std::powf(bullet->GetTrackingData().baseSpeed, 2) / bullet->GetTrackingData().lerpRadius;
	// 力の向き
	Vector3 force = centripetalAccel * maxCentripetalForce;
	// 推進力計算
	float propulsion = bullet->GetTrackingData().baseSpeed * bullet->GetTrackingData().damping;
	// 向心力に現在の方向ベクトルに＋推進力でベクトルを作成
	force += nowDirect * propulsion;
	// 速度の減衰処理
	force -= bulletVelocity * bullet->GetTrackingData().damping;

	return Vector3(force * GameSystem::GameSpeedFactor());
}

Vector3 TrackingMoveState::CalcGeniusAcceleration()
{
	// キャストポインタ
	TrackingBullet* bullet = dynamic_cast<TrackingBullet*>(bullet_);

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

	float maxCentripetalForce = std::powf(bullet->GetTrackingData().baseSpeed, 2) / bullet->GetTrackingData().lerpRadius;

	Vector3 force = centripetalAccel * maxCentripetalForce;

	float propulsion = bullet->GetTrackingData().baseSpeed * bullet->GetTrackingData().damping;

	force += nowDirect * propulsion;
	force -= bulletVelocity * bullet->GetTrackingData().damping;

	return Vector3(force * GameSystem::GameSpeedFactor());
}
