#include "TrackingMoveState.h"
#include "Application/GameSystem/GameSystem.h"
#include "Application/GameObject/IGameObject.h"
#include "../../BulletManager.h"
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
	float looseFrame = 150.0f;	// 緩くなるまでの時間
	looseTimer_.Start(looseFrame);
	if (dynamic_cast<TrackingBullet*>(bullet_)) {
		TrackingBullet* bullet = dynamic_cast<TrackingBullet*>(bullet_);
		accelerationTime_.Start(100.0f);
		if (bullet->GetTrackingData().accelFrame > 10) {
			accelerationTime_.Start(bullet->GetTrackingData().accelFrame);
		}
	}
	

	elapsedTime_ = 0.0f;
	currentFrame_ = 0.0f;
}

void TrackingMoveState::Update(BulletStateMachine& stateMachine)
{
	// タイマーの更新
	timer_.Update();
	accelerationTime_.Update();

	// 誘導弾なら
	if (dynamic_cast<TrackingBullet*>(bullet_)) {
		// 対象が居なければ
		if (!bullet_->GetTarget()) {
			return;
		}
		// 種類の受け取り
		TrackingAttribute type = dynamic_cast<TrackingBullet*>(bullet_)->GetTrackingType();

		// 向きが過度に離れていたら追尾しない
		float dot = Vector3::Dot(Vector3::Normalize(bullet_->GetVelocity()), Vector3::Normalize(bullet_->GetTarget()->worldTransform_.GetWorldPosition() - bullet_->GetWorldPosition()));
		float limitDot = GlobalVariables::GetInstance()->GetValue<float>("BossTrackingBullet", "TrackingDot");
		limitDot = GlobalVariables::GetInstance()->GetValue<float>("BossTrackingBullet", "TrackingDotLoose");
		// 追従を厳しくするタイマーがfalseなら
		if (looseTimer_.IsActive()) {
			limitDot = GlobalVariables::GetInstance()->GetValue<float>("BossTrackingBullet", "TrackingDot");
			// 追従をキャンセル
			if (dot < limitDot) {
				dynamic_cast<TrackingBullet*>(bullet_)->SetStraightFrame(300.0f);
				stateMachine.RequestState(TrackingState::kStraight);
				return;
			}
		}
		// true = 緩い
		else {
			float supLimit = 0.05f;
			float infLimit = 0.2f;
			float genLimit = -0.05f;
			switch (type)
			{
			case TrackingAttribute::kSuperior:
				if (dot < supLimit) {
					dynamic_cast<TrackingBullet*>(bullet_)->SetStraightFrame(300.0f);
					stateMachine.RequestState(TrackingState::kStraight);
					return;
				}
				break;
			case TrackingAttribute::kInferior:
				if (dot < infLimit) {
					dynamic_cast<TrackingBullet*>(bullet_)->SetStraightFrame(300.0f);
					stateMachine.RequestState(TrackingState::kStraight);
					return;
				}
				break;
			case TrackingAttribute::kGenius:
				if (dot < genLimit) {
					dynamic_cast<TrackingBullet*>(bullet_)->SetStraightFrame(300.0f);
					stateMachine.RequestState(TrackingState::kStraight);
					return;
				}
				break;
			case TrackingAttribute::kNone:
				break;
			case TrackingAttribute::kMaxSize:
				break;
			default:
				// 追従をキャンセル
				if (dot < limitDot) {
					dynamic_cast<TrackingBullet*>(bullet_)->SetStraightFrame(300.0f);
					stateMachine.RequestState(TrackingState::kStraight);
					return;
				}
				break;
			}

		}
		// ベクトルの正規化
		Vector3 toDirect = CalculateDirection(type).Normalize();
		// 処理の種類
		int number = BulletManager::sTrackingProcessType;
		// 加速度初期化
		parentAcceleration_ = {};
		switch (number)
		{
		case 0:
			// 加速度の計算
			parentAcceleration_ = accelerater_->CalcTrackingAcceleration(toDirect, accelerationTime_);
			break;
		case 1:
			// 一定フレームでのみ更新
			currentFrame_++;
			//float refreshFrame = 15.0f;
			if (currentFrame_ >= BulletManager::sTrackingRefreshFrame) {
				// 加速度の計算
				parentAcceleration_ = accelerater_->CalcTrackingAcceleration(toDirect, accelerationTime_);
				// 加速度の設定
				currentFrame_ = 0.0f;
			}
			break;
		case 2:
			if (frameCount_ >= 1024) {
				frameCount_ = 0;
			}
			else if ((frameCount_ > 0) && ((frameCount_ & (frameCount_ - 1)) == 0)) {
				// 加速度の計算
				parentAcceleration_ = accelerater_->CalcTrackingAcceleration(toDirect, accelerationTime_);
			}
			frameCount_++;
			break;
		default:
			// 加速度の計算
			parentAcceleration_ = accelerater_->CalcTrackingAcceleration(toDirect, accelerationTime_);
			break;
		}

		bullet_->SetAccelerate(parentAcceleration_);
	}
}

void TrackingMoveState::Exit()
{

}

void TrackingMoveState::CheckPassing()
{

	

}

Vector3 TrackingMoveState::CalculateDirection(TrackingAttribute type)
{
#pragma region 波の処理（使ってない
	//---揺れる処理---//
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
#pragma endregion

	// 方向ベクトル
	Vector3 toDirect = {};
	//---劣等---//
	Vector3 offsetPoint = bullet_->GetTarget()->worldTransform_.GetWorldPosition() + inferiorOffset_;
	//---秀才---//
	// プレイヤーの現在の位置と速度
	Vector3 targetPoint = bullet_->GetTarget()->worldTransform_.GetWorldPosition();
	Vector3 playerDirection = targetPoint - bullet_->GetTarget()->prevPosition_;
	Vector3 predictedPosition{};	// 予測先の座標
	// 予測位置を計算
	float predictionTime = GlobalVariables::GetInstance()->GetValue<float>("TrackSuperior", "PredictionTime"); // ミサイルが向かう予測時間

	// 種類ごとの計算
	switch (type)
	{
	case TrackingAttribute::kSuperior:	// 優等
		toDirect = bullet_->GetTarget()->worldTransform_.GetWorldPosition() - bullet_->GetWorldPosition();
		break;
	case TrackingAttribute::kInferior:	// 劣等
		toDirect = offsetPoint - bullet_->GetWorldPosition();
		break;
	case TrackingAttribute::kGenius:	// 秀才
		// 予測先の計算
		if (playerDirection.x == 0.0f && playerDirection.y == 0.0f && playerDirection.z == 0.0f) {
			predictedPosition = targetPoint;
		}
		else {
			predictedPosition = targetPoint + (playerDirection * predictionTime);
		}
		// 予測先に対するベクトル
		toDirect = predictedPosition - bullet_->GetWorldPosition();
		break;
	default:
		break;
	}

	return Vector3(toDirect.Normalize());
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

