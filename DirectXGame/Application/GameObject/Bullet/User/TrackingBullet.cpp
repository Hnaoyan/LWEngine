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
		// 直進の時間設定
		straightTimer_.Start(instance->GetValue<float>("BossTrackingBullet", "StraightFrame"));
	
		isTargetBoss_ = false;
	}
	else if (boss) {
		sTrackingFrame = instance->GetValue<float>("PlayerTrackingBullet", "TrackFrame");
		sDamping = instance->GetValue<float>("PlayerTrackingBullet", "Damping");
		sBaseVelocity = instance->GetValue<float>("PlayerTrackingBullet", "BaseSpeed");
		sInitSpeed = instance->GetValue<float>("PlayerTrackingBullet", "InitSpeed");
		sLerpRadius = instance->GetValue<float>("PlayerTrackingBullet", "LerpRadius");
		// 直進の時間設定
		straightTimer_.Start(instance->GetValue<float>("PlayerTrackingBullet", "StraightFrame"));
		
		isTargetBoss_ = true;
	}

	
	nowState_ = TrackingState::kStraight;

	stateMachine_->ChangeRequest(std::make_unique<TrackingStraightState>());

}

void TrackingBullet::Update()
{
	// 追尾タイマー
	straightTimer_.Update();
	trackTimer_.Update();
	waveTimer_.Update();

	if (straightTimer_.IsEnd()) {
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
	if (waveTimer_.IsEnd()) {
		requestState_ = TrackingState::kTracking;
	}
	if (requestState_) {

		nowState_ = requestState_.value();

		switch (nowState_)
		{
		case TrackingState::kStraight:
			straightTimer_.Start(GlobalVariables::GetInstance()->GetValue<float>("BossTrackingBullet", "StraightFrame"));
			stateMachine_->RequestState(TrackingState::kStraight);
			break;
		case TrackingState::kWave:
			waveTimer_.Start(90.0f);
			waveCount_ = 0.0f;
			stateMachine_->RequestState(TrackingState::kWave);
			break;
		case TrackingState::kTracking:
			trackTimer_.Start(TrackingBullet::sTrackingFrame);
			stateMachine_->RequestState(TrackingState::kTracking);
			break;
		default:
			break;
		}

		// リクエストリセット
		requestState_ = std::nullopt;
	}

	switch (nowState_)
	{
	case TrackingState::kStraight:

		break;
	case TrackingState::kWave:
		WaveUpdate();
		break;
	case TrackingState::kTracking:
		// 追尾の速度計算処理
		//TrackUpdate();
		break;
	default:
		break;
	}

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
	if (isDead_ && breakEmitter_) {
		static_cast<BulletParticle::BreakEffect*>(breakEmitter_)->SetPosition(GetWorldPosition());
		//static_cast<BulletParticle::BreakEffect*>(breakEmitter_)->SetEmitFlag(true);
	}
}

void TrackingBullet::WaveUpdate()
{
	//Vector3 normalizeVelocity = Vector3::Normalize(velocity_);
	// 横方向のベクトル
	//if (Vector3::Length(sideVector) == 0) {
	//	sideVector = Vector3::Cross(normalizeVelocity, Vector3(0.0f, 0.0f, 1.0f));
	//}
	//Vector3 sideVector = Vector3::Cross(normalizeVelocity, Vector3(0.0f, 1.0f, 0.0f));
	//sideVector.Normalize();

	//// 縦方向のベクトル
	//Vector3 upVector = Vector3::Cross(sideVector, normalizeVelocity);
	//upVector.Normalize();

	//// 揺れの計算
	//float frequency = 1.0f;
	//float swingAmount = 1.0f;
	//waveCount_ += (1.0f / 5.0f);
	//float t = waveCount_ * frequency;
	//Vector2 swing = { std::sinf(t) * swingAmount,std::cosf(t) * swingAmount };
	//Vector3 swing3D = (sideVector * swing.x) + (upVector * swing.y);

	//accelerate_ = normalizeVelocity + swing3D;

	Vector3 velocity = velocity_.Normalize();

	Vector3 worldUp = Vector3::Up();
	Vector3 side = Vector3::Normalize(Vector3::Cross(worldUp, velocity));
	Vector3 up = Vector3::Normalize(Vector3::Cross(velocity, side));

	float frequency = 0.5f;
	float maxAmount = 10.0f;
	waveCount_ += (1.0f / 2.0f);
	float t = waveCount_ * frequency;
	float swayX = std::sinf(t) * maxAmount;

	Vector3 sway = side * swayX;

	Vector3 newDirect = velocity + sway;
	newDirect = Vector3::Normalize(newDirect);
	accelerate_ = newDirect;
}
