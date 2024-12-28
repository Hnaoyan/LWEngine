#include "TrackingMoveState.h"
#include "Application/GameSystem/GameSystem.h"
#include "Application/GameObject/IGameObject.h"
#include "Engine/GlobalVariables/GlobalVariables.h"

#include "../../BulletsPaths.h"
#include "../StateMachine/BulletStateMachine.h"

void TrackingMoveState::Enter()
{
	GlobalVariables* global = GlobalVariables::GetInstance();
	// 加速度計算クラス生成
	accelerater_ = std::make_unique<TrackingAccelerater>(dynamic_cast<TrackingBullet*>(bullet_));
	
	// 初期化
	inferiorOffset_ = {};
	// ずらすオフセット作成
	std::string groupName = "TrackInferior";
	float offsetValue = global->GetValue<float>(groupName, "MaxOffset");
	// 最小値
	float limit = global->GetValue<float>(groupName, "MinOffset");
	// オフセット
	inferiorOffset_ = LwLib::GetRandomValue({ -offsetValue,-offsetValue,-offsetValue }, { offsetValue,offsetValue,offsetValue }, limit);

	//// 減衰率
	//float damping = 1.0f / 300.0f;
	//Vector3 newVelocity = bullet_->GetVelocity() + bullet_->GetVelocity() * (damping);
	//bullet_->SetVelocity(newVelocity);

	// 1.5秒で追従を緩くする（仮
	float looseFrame = 90.0f;	// 緩くなるまでの時間
	looseTimer_.Start(looseFrame);
	accelerationTime_.Start(240.0f);

	elapsedTime_ = 0.0f;
}

void TrackingMoveState::Update(BulletStateMachine& stateMachine)
{
	// タイマーの更新
	timer_.Update();
	accelerationTime_.Update();

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

		if (!looseTimer_.IsActive()) {
			limitDot = GlobalVariables::GetInstance()->GetValue<float>("BossTrackingBullet", "TrackingDotLoose");
		}

		if (dot < limitDot) {
			stateMachine.RequestState(TrackingState::kStraight);
			return;
		}

		// 種類の受け取り
		TrackingAttribute type = dynamic_cast<TrackingBullet*>(bullet_)->GetTrackingType();
		// 時間
		float addTime = 1.0f / 5.0f; // 毎フレーム加算する時間
		elapsedTime_ += addTime;
		float frequency = 1.0f;	// 間隔
		float amplitude = 10.0f;	// 振幅
		//float damping = 0.75f;	// 速度の減衰率
		float maxTime = 1.0e6f;	// バグ回避用の最大時間
		if (elapsedTime_ >= maxTime) {
			elapsedTime_ = 0.0f;
		}
		// オフセット
		float offset = std::sinf(elapsedTime_ * frequency) * amplitude;
		offset *= 3.0f;
		// 垂直ベクトル
		Vector3 crossDirect = Vector3::Cross(bullet_->GetVelocity().Normalize(), Vector3::Right());
		// 方向ベクトル
		Vector3 toDirect = bullet_->GetTarget()->worldTransform_.GetWorldPosition() - bullet_->GetWorldPosition();

		//---劣等---//
		Vector3 offsetPoint = bullet_->GetTarget()->worldTransform_.GetWorldPosition() + inferiorOffset_;


		//---秀才---//
		// プレイヤーの現在の位置と速度
		Vector3 targetPoint = bullet_->GetTarget()->worldTransform_.GetWorldPosition();
		Vector3 playerDirection = bullet_->GetTarget()->worldTransform_.GetWorldPosition() - bullet_->GetTarget()->prevPosition_;
		Vector3 predictedPosition{};	// 予測先の座標
		// 予測位置を計算
		float predictionTime = GlobalVariables::GetInstance()->GetValue<float>("TrackSuperior", "PredictionTime"); // ミサイルが向かう予測時間

		// 種類ごとの計算
		switch (type)
		{
		case TrackingAttribute::kSuperior:	// 優等
			//// 親加速度計算
			//parentAcceleration_ = CalcSuperiorAcceleration();
			//// 子加速度計算
			//childAcceleration_ = crossDirect.Normalize() * offset;
			//parentAcceleration_ += childAcceleration_;
			//parentAcceleration_ *= damping;
			//parentAcceleration_ *= 5.0f;
			//parentAcceleration_ = accelerater_->CalcSuperiorAcceleration();

			//parentAcceleration_ = accelerater_->CalcTrackingAcceleration(toDirect);

			break;
		case TrackingAttribute::kInferior:	// 劣等
			//parentAcceleration_ = CalcInferiorAcceleration();
			//parentAcceleration_ = accelerater_->CalcInferiorAcceleration(this->inferiorOffset_);

			toDirect = offsetPoint - bullet_->GetWorldPosition();

			break;
		case TrackingAttribute::kGenius:	// 秀才
			//parentAcceleration_ = CalcGeniusAcceleration();
			// 予測先の計算
			if (playerDirection.x == 0.0f && playerDirection.y == 0.0f && playerDirection.z == 0.0f) {
				predictedPosition = targetPoint;
			}
			else {
				predictedPosition = targetPoint + (playerDirection * predictionTime);
			}

			toDirect = predictedPosition - bullet_->GetWorldPosition();
			//parentAcceleration_ = accelerater_->CalcGeniusAcceleration();
			break;
		default:
			break;
		}
		toDirect.Normalize();
		parentAcceleration_ = accelerater_->CalcTrackingAcceleration(toDirect, accelerationTime_);

		// 加速度の設定
		bullet_->SetAccelerate(parentAcceleration_);
	}

}

void TrackingMoveState::Exit()
{

}

Vector3 TrackingMoveState::CalcSuperiorAcceleration()
{
	// キャストポインタ
	TrackingBullet* bullet = dynamic_cast<TrackingBullet*>(bullet_);
	//float speed = bullet->GetTrackingData().baseSpeed + (1.0f / 5.0f);
	float speed = bullet->GetTrackingData().baseSpeed;
	float maxSpeed = bullet->GetTrackingData().baseSpeed + 250.0f;
	if (accelerationTime_.IsActive()) {
		speed = Ease::Easing(speed, maxSpeed, accelerationTime_.GetElapsedFrame());
	}
	else {
		speed = maxSpeed;
	}

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
	float maxCentripetalAccel = 5.0f;

	if (centripetalAccelMagnitude > maxCentripetalAccel) {
		centripetalAccel /= centripetalAccelMagnitude;
	}
	// 最大向心力
	float maxCentripetalForce = std::powf(speed, 2) / bullet->GetTrackingData().lerpRadius;

	// 力の向き
	Vector3 force = centripetalAccel * maxCentripetalForce;
	// 推進力計算
	float propulsion = speed * bullet->GetTrackingData().damping;
	// 向心力に現在の方向ベクトルに＋推進力でベクトルを作成
	force += nowDirect * propulsion;
	// 速度の減衰処理
	force -= bulletVelocity * bullet->GetTrackingData().damping;

	// 加速度の計算
	return Vector3(force);
}

