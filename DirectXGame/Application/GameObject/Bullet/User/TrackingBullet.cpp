#include "TrackingBullet.h"
#include "Application/Collision/ColliderFilter.h"
#include "Application/GameSystem/GameSystem.h"
#include "Application/GameObject/GameObjectLists.h"
#include "Engine/LwLib/LwEngineLib.h"
#include "Engine/GlobalVariables/GlobalVariables.h"
#include "TrackingState/TrackingStates.h"
#include "Application/GameObject/Particle/User/ParticleLists.h"

float TrackingBullet::sTrackingFrame = 95.0f;
float TrackingBullet::sDamping = 0.1f;
float TrackingBullet::sBaseVelocity = 90.0f;
float TrackingBullet::sInitSpeed = 70.0f;
float TrackingBullet::sLerpRadius = 50.0f;

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
		sTrackingFrame = instance->GetValue<float>("BossTrackingBullet", "TrackFrame");
		sDamping = instance->GetValue<float>("BossTrackingBullet", "Damping");
		sBaseVelocity = instance->GetValue<float>("BossTrackingBullet", "BaseSpeed");
		sInitSpeed = instance->GetValue<float>("BossTrackingBullet", "InitSpeed");
		sLerpRadius = instance->GetValue<float>("BossTrackingBullet", "LerpRadius");
		data_.LoadGlobalData("BossTrackingBullet");
		// 直進の時間設定
		straightFrame_ = instance->GetValue<float>("BossTrackingBullet", "StraightFrame");
		isTargetBoss_ = false;
	}
	else if (boss) {
		sTrackingFrame = instance->GetValue<float>("PlayerTrackingBullet", "TrackFrame");
		sDamping = instance->GetValue<float>("PlayerTrackingBullet", "Damping");
		sBaseVelocity = instance->GetValue<float>("PlayerTrackingBullet", "BaseSpeed");
		sInitSpeed = instance->GetValue<float>("PlayerTrackingBullet", "InitSpeed");
		sLerpRadius = instance->GetValue<float>("PlayerTrackingBullet", "LerpRadius");
		data_.LoadGlobalData("PlayerTrackingBullet");
		// 直進の時間設定
		straightFrame_ = (instance->GetValue<float>("PlayerTrackingBullet", "StraightFrame"));
		
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

	ChangeSelecter();

	// ステートの処理
	stateMachine_->Update();

	// 移動
	velocity_ += accelerate_;
	// 回転
	transform_.rotate.x += GameSystem::GameSpeedFactor() * 3.0f;
	transform_.rotate.y += GameSystem::GameSpeedFactor() * 2.0f;

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
	// 
	if (isDead_ && breakEmitter_) {
		//static_cast<BulletParticle::BreakEffect*>(breakEmitter_)->SetPosition(GetWorldPosition());
		//static_cast<BulletParticle::BreakEffect*>(breakEmitter_)->SetEmitFlag(true);
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
	if (isTargetBoss_) {
		if (trackTimer_.IsEnd()) {
			requestState_ = TrackingState::kWave;
		}
	}
	else {
		if (trackTimer_.IsEnd()) {
			requestState_ = TrackingState::kStraight;
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
			if (object_) {
				float dot = Vector3::Dot(Vector3::Normalize(GetVelocity()), Vector3::Normalize(object_->worldTransform_.GetWorldPosition() - GetWorldPosition()));
				// 向きが過度に離れていたら追尾しない
				float limitDot = GlobalVariables::GetInstance()->GetValue<float>("BossTrackingBullet", "TrackingDot");
				if (dot < limitDot + 0.025f) {
					straightTimer_.Start(GlobalVariables::GetInstance()->GetValue<float>("BossTrackingBullet", "StraightFrame"));
					stateMachine_->RequestState(TrackingState::kStraight);
				}
				else {
					trackTimer_.Start(TrackingBullet::sTrackingFrame);
					stateMachine_->RequestState(TrackingState::kTracking);
				}
			}
			else {
				trackTimer_.Start(TrackingBullet::sTrackingFrame);
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
