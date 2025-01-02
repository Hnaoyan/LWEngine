#include "StateTransitionManager.h"
#include "Application/GameObject/GameObjectLists.h"
#include "Application/GameObject/Bullet/BulletsPaths.h"
#include "Engine/GlobalVariables/GlobalVariables.h"
#include <cassert>

void StateTransitionManager::Initialize(TrackingBullet* bullet)
{
	assert(bullet);
	// マシンの設定
	bullet_ = bullet;
	isTargetBoss_ = bullet_->IsTargetBoss();
	if (isTargetBoss_) {
		transitionTimer_.Start(
			GlobalVariables::GetInstance()->GetValue<float>("BossTrackingBullet", "StraightFrame"));
	}
	else {
		transitionTimer_.Start(
			GlobalVariables::GetInstance()->GetValue<float>("PlayerTrackingBullet", "StraightFrame"));
	}
	isCount_ = false;
}

void StateTransitionManager::Update()
{
	transitionTimer_.Update();

	// 遷移タイマーで変更
	if (transitionTimer_.IsEnd()) {
		TrackingState state = bullet_->GetStateMachine()->GetNowState();
		switch (state)
		{
		case TrackingState::kStraight:
			//requestState_ = TrackingState::kTurnToTarget;
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
		Player* player = dynamic_cast<Player*>(bullet_->GetTarget());
		if (player->GetSystemFacede()->GetDudgeManager()->IsInvisibleActive()) {
			// 追従解除処理
			float maxDistance = player->GetTrackCancelDistance();
			float bulletToPlayer = bullet_->TargetToDistance();
			if (bulletToPlayer <= maxDistance) {
				transitionTimer_.End();
				// ジャスト回避時のみ例外処理
				transitionTimer_.Start(bullet_->GetTrackingData().straightFrame * 2.5f);
				bullet_->GetStateMachine()->RequestState(TrackingState::kStraight);
				if (!isCount_) {
					// 振り切りカウント
					player->GetSystemFacede()->GetDudgeManager()->DodgeCountIncrement();
					isCount_ = true;
				}
			}
		}
		else {
			isCount_ = false;
		}
	}

	// リクエスト処理
	if (requestState_) {
		switch (requestState_.value())
		{
		case TrackingState::kStraight:
			transitionTimer_.Start(bullet_->GetTrackingData().straightFrame);
			bullet_->GetStateMachine()->RequestState(TrackingState::kStraight);
			break;
		case TrackingState::kWave:
			transitionTimer_.Start(90.0f);
			bullet_->GetStateMachine()->RequestState(TrackingState::kWave);
			break;
		case TrackingState::kTracking:
			// 対象がある場合
			if (bullet_->GetTarget()) {
				float dot = Vector3::Dot(Vector3::Normalize(bullet_->GetVelocity()), Vector3::Normalize(bullet_->GetTarget()->worldTransform_.GetWorldPosition() - bullet_->GetWorldPosition()));
				// 向きが過度に離れていたら追尾しない
				float limitDot = GlobalVariables::GetInstance()->GetValue<float>("BossTrackingBullet", "TrackingDot");
				// 追従しないのに切り替えの場合
				if (dot < limitDot + 0.025f) {
					transitionTimer_.Start(bullet_->GetTrackingData().straightFrame);
					bullet_->GetStateMachine()->RequestState(TrackingState::kStraight);
				}
				// 変わらず追従
				else {
					if (bullet_->GetStateMachine()->GetChangeCount() > 5) {
						transitionTimer_.Start(bullet_->GetTrackingData().straightFrame);
						bullet_->GetStateMachine()->RequestState(TrackingState::kStraight);
					}
					transitionTimer_.Start(bullet_->GetTrackingData().trackFrame);
					bullet_->GetStateMachine()->RequestState(TrackingState::kTracking);
				}
			}
			// ない場合
			else {
				transitionTimer_.Start(bullet_->GetTrackingData().trackFrame);
				bullet_->GetStateMachine()->RequestState(TrackingState::kTracking);
			}
			break;
		case TrackingState::kTurnToTarget:
			//transitionTimer_.Start(90.0f);
			bullet_->GetStateMachine()->RequestState(TrackingState::kTurnToTarget);
			break;
		default:
			break;
		}

		// リクエストリセット
		requestState_ = std::nullopt;
	}

}
