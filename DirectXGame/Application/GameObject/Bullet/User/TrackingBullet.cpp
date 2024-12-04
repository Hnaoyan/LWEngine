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
	Boss* boss = dynamic_cast<Boss*>(object_);
	Player* player = dynamic_cast<Player*>(object_);
	GlobalVariables* instance = GlobalVariables::GetInstance();
	if (player) {
		data_.LoadGlobalData("BossTrackingBullet");
		// 直進の時間設定
		straightFrame_ = instance->GetValue<float>("BossTrackingBullet", "StraightFrame");
		// 対象がボスか
		isTargetBoss_ = false;
	}
	else if (boss) {
		data_.LoadGlobalData("PlayerTrackingBullet");
		// 直進の時間設定
		straightFrame_ = (instance->GetValue<float>("PlayerTrackingBullet", "StraightFrame"));
		// 対象がボスか
		isTargetBoss_ = true;
	}

	// 直進の時間をランダムにする処理
	if (!isBarrage_) {
		straightFrame_ = LwLib::GetRandomValue(straightFrame_, straightFrame_ + 60.0f);
	}

	straightTimer_.Start(straightFrame_);

	// ステートの設定
	stateMachine_ = std::make_unique<BulletStateMachine>(this);
	stateMachine_->RequestState(TrackingState::kStraight);
}

void TrackingBullet::Update()
{
	// それぞれのタイマー
	straightTimer_.Update();
	trackTimer_.Update();
	waveTimer_.Update();

	trackCoolTime_.Update();	// 追従しない時間
	trackingTime_.Update();	// 追従する時間

	ChangeSelecter();

	// ステートの処理
	stateMachine_->Update(trackingTime_.IsActive());
	// 追従の時間関係
	if (trackingTime_.IsEnd()) {
		float randomCoolTime = LwLib::GetRandomValue(5.0f, 10.0f);
		trackCoolTime_.Start(randomCoolTime);
	}
	if (trackCoolTime_.IsEnd()) {
		trackingTime_.Start();
	}
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
	// 進行方向とターゲット方向の向き
	// 直接的な距離
	// 加速度と速度を

	if (straightTimer_.IsEnd()) {
		requestState_ = TrackingState::kTracking;
	}
	if (waveTimer_.IsEnd()) {
		requestState_ = TrackingState::kTracking;
	}
	// ボスなら
	if (isTargetBoss_) {
		if (trackTimer_.IsEnd()) {
			requestState_ = TrackingState::kWave;
		}
	}
	// プレイヤーなら
	else {
		if (trackTimer_.IsEnd()) {
			requestState_ = TrackingState::kStraight;
		}
		Player* player = dynamic_cast<Player*>(object_);

		if (player->GetSystemFacede()->GetDudgeManager()->IsInvisibleActive()) {
			float maxDistance = player->trackCancelDistance;
			float bulletToPlayer = Vector3::Distance(GetWorldPosition(), player->worldTransform_.GetWorldPosition());
			if (bulletToPlayer <= maxDistance) {
				straightTimer_.End();
				waveTimer_.End();
				trackTimer_.End();
				// ジャスト回避時のみ例外処理
				straightTimer_.Start(GlobalVariables::GetInstance()->GetValue<float>("BossTrackingBullet", "StraightFrame") * 2.5f);
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
			straightTimer_.Start(GlobalVariables::GetInstance()->GetValue<float>("BossTrackingBullet", "StraightFrame"));	
			stateMachine_->RequestState(TrackingState::kStraight);
			break;
		case TrackingState::kWave:
			waveTimer_.Start(90.0f);
			stateMachine_->RequestState(TrackingState::kWave);
			break;
		case TrackingState::kTracking:
			// 対象がある場合
			if (object_) {
				float dot = Vector3::Dot(Vector3::Normalize(GetVelocity()), Vector3::Normalize(object_->worldTransform_.GetWorldPosition() - GetWorldPosition()));
				// 向きが過度に離れていたら追尾しない
				float limitDot = GlobalVariables::GetInstance()->GetValue<float>("BossTrackingBullet", "TrackingDot");
				// 追従しないのに切り替えの場合
				if (dot < limitDot + 0.025f) {
					straightTimer_.Start(GlobalVariables::GetInstance()->GetValue<float>("BossTrackingBullet", "StraightFrame"));
					stateMachine_->RequestState(TrackingState::kStraight);
				}
				// 変わらず追従
				else {
					if (stateMachine_->GetChangeCount() > 5) {
						straightTimer_.Start(GlobalVariables::GetInstance()->GetValue<float>("BossTrackingBullet", "StraightFrame"));
						stateMachine_->RequestState(TrackingState::kStraight);
					}
					trackTimer_.Start(GlobalVariables::GetInstance()->GetValue<float>("BossTrackingBullet", "TrackFrame"));
					stateMachine_->RequestState(TrackingState::kTracking);
				}
			}
			// ない場合
			else {
				trackTimer_.Start(GlobalVariables::GetInstance()->GetValue<float>("BossTrackingBullet", "TrackFrame"));
				stateMachine_->RequestState(TrackingState::kTracking);
			}
			trackingTime_.Start();
			break;
		default:
			break;
		}

		// リクエストリセット
		requestState_ = std::nullopt;
	}
}
