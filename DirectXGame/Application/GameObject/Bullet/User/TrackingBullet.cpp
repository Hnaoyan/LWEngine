#include "TrackingBullet.h"
#include "Application/Collision/ColliderFilter.h"
#include "Application/GameSystem/GameSystem.h"
#include "Application/GameObject/GameObjectLists.h"
#include "Engine/LwLib/LwLibrary.h"
#include "Engine/GlobalVariables/GlobalVariables.h"
#include "TrackingState/TrackingStates.h"
#include "Application/GameObject/Particle/User/ParticlePaths.h"

void TrackingBullet::Initialize()
{
	// 基底の初期化・コライダーのマスク設定
	IBullet::Initialize();
	collider_.SetAttribute(kCollisionAttributeEnemyBullet);

	//if (object_->GetTag());
	Boss* boss = dynamic_cast<Boss*>(targetObject_);
	Player* player = dynamic_cast<Player*>(targetObject_);
	GlobalVariables* instance = GlobalVariables::GetInstance();
	// 追従対象がプレイヤー
	if (player) {
		data_.LoadGlobalData("BossTrackingBullet");
		// 直進の時間設定
		//straightFrame_ = instance->GetValue<float>("BossTrackingBullet", "StraightFrame");
		isTargetBoss_ = false;
	}
	// 追従対象がボス
	else if (boss) {
		data_.LoadGlobalData("PlayerTrackingBullet");
		// 直進の時間設定
		straightFrame_ = (instance->GetValue<float>("PlayerTrackingBullet", "StraightFrame"));
		// 対象がボスか
		isTargetBoss_ = true;
	}

	// 速度を距離に応じて
	float reductionRate = 0.5f;	// 減速するレート
	float reductionDistance = 175.0f;	// 制限距離
	float redMinDistance = 75.0f;
	float toDistance = TargetToDistance();
	if (TargetToDistance() <= reductionDistance) {
		float v = (toDistance - redMinDistance) / (reductionDistance - redMinDistance);
		v = std::clamp(v, 0.0f, 1.0f);
		reductionRate = Ease::Easing(0.5f, 1.0f, v);
		//data_.initSpeed *= reductionRate;
		//data_.baseSpeed *= reductionRate;
		velocity_ *= reductionRate;
	}

	transitionTimer_.Start(straightFrame_);
	// ステートの設定
	stateMachine_ = std::make_unique<BulletStateMachine>(this);
	stateMachine_->RequestState(TrackingState::kStraight);
}

void TrackingBullet::Update()
{
	// 遷移時間
	transitionTimer_.Update();

	ChangeSelecter();

	// ステートの処理
	stateMachine_->Update(true);

	// 移動
	velocity_ += accelerate_ * GameSystem::GameSpeedFactor();
	// 回転
	transform_.rotate.x += GameSystem::GameSpeedFactor();
	transform_.rotate.y += GameSystem::GameSpeedFactor();

	// 基底の更新
	IBullet::Update();
}

void TrackingBullet::ImGuiDraw()
{

}

void TrackingBullet::OnCollision(ColliderObject object)
{
	// 地形
	if (std::holds_alternative<Terrain*>(object)) {
		isDead_ = true;
	}
	// プレイヤー
	if (std::holds_alternative<Player*>(object)) {
		isDead_ = true;
	}
	// 
	if (std::holds_alternative<Boss*>(object)) {
		isDead_ = true;
	}
}

void TrackingBullet::ChangeSelecter()
{
	// 遷移タイマーで変更
	if (transitionTimer_.IsEnd()) {
		TrackingState state = stateMachine_->GetNowState();
		switch (state)
		{
		case TrackingState::kStraight:
			requestState_ = TrackingState::kTracking;
			break;
		case TrackingState::kWave:
			requestState_ = TrackingState::kTracking;
			break;
		case TrackingState::kTracking:
			if (isTargetBoss_) {
				requestState_ = TrackingState::kWave;
			}
			else {
				requestState_ = TrackingState::kStraight;
			}
			break;
		}
	}
	// プレイヤーへ移動
	if (!isTargetBoss_) {
		Player* player = dynamic_cast<Player*>(targetObject_);
		if (player->GetSystemFacede()->GetDudgeManager()->IsInvisibleActive()) {
			// 追従解除処理
			float maxDistance = player->GetTrackCancelDistance();
			float bulletToPlayer = TargetToDistance();
			if (bulletToPlayer <= maxDistance) {
				transitionTimer_.End();
				// ジャスト回避時のみ例外処理
				transitionTimer_.Start(data_.straightFrame * 2.5f);
				stateMachine_->RequestState(TrackingState::kStraight);
				if (!isCount) {
					// 振り切りカウント
					player->GetSystemFacede()->GetDudgeManager()->DodgeCountIncrement();
					isCount = true;
				}
			}
		}
		else {
			isCount = false;
		}
	}

	// リクエスト処理
	if (requestState_) {
		switch (requestState_.value())
		{
		case TrackingState::kStraight:
			transitionTimer_.Start(data_.straightFrame);
			stateMachine_->RequestState(TrackingState::kStraight);
			break;
		case TrackingState::kWave:
			transitionTimer_.Start(90.0f);
			stateMachine_->RequestState(TrackingState::kWave);
			break;
		case TrackingState::kTracking:
			// 対象がある場合
			if (targetObject_) {
				float dot = Vector3::Dot(Vector3::Normalize(GetVelocity()), Vector3::Normalize(targetObject_->worldTransform_.GetWorldPosition() - GetWorldPosition()));
				// 向きが過度に離れていたら追尾しない
				float limitDot = GlobalVariables::GetInstance()->GetValue<float>("BossTrackingBullet", "TrackingDot");
				// 追従しないのに切り替えの場合
				if (dot < limitDot + 0.025f) {
					transitionTimer_.Start(data_.straightFrame);
					stateMachine_->RequestState(TrackingState::kStraight);
				}
				// 変わらず追従
				else {
					if (stateMachine_->GetChangeCount() > 5) {
						transitionTimer_.Start(data_.straightFrame);
						stateMachine_->RequestState(TrackingState::kStraight);
					}
					transitionTimer_.Start(data_.trackFrame);
					stateMachine_->RequestState(TrackingState::kTracking);
				}
			}
			// ない場合
			else {
				transitionTimer_.Start(data_.trackFrame);
				stateMachine_->RequestState(TrackingState::kTracking);
			}
			break;
		default:
			break;
		}

		// リクエストリセット
		requestState_ = std::nullopt;
	}
}
