#include "TrackingBullet.h"
#include "Application/Collision/ColliderFilter.h"
#include "Application/GameSystem/GameSystem.h"
#include "Application/GameObject/GameObjectLists.h"
#include "Engine/LwLib/LwEngineLib.h"
#include "Engine/GlobalVariables/GlobalVariables.h"
#include "TrackingState/TrackingStates.h"

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
	// タイプごとの初期化
	SetupByType();

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
			break;
		case TrackingState::kWave:
			waveTimer_.Start(90.0f);
			waveCount_ = 0.0f;
			accelerate_ = {};
			break;
		case TrackingState::kTracking:
			trackTimer_.Start(TrackingBullet::sTrackingFrame);
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
		TrackUpdate();
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
}

void TrackingBullet::SetupByType()
{
	GlobalVariables* global = GlobalVariables::GetInstance();
	// 初期化
	offset_ = {};
	// ずらすオフセット作成
	std::string groupName = "TrackInferior";
	float offsetValue = global->GetValue<float>(groupName, "MaxOffset");
	// 最小値
	float limit = global->GetValue<float>(groupName, "MinOffset");

	switch (trackingType_)
	{
	case TrackingType::kStandard:

		break;
	case TrackingType::kInferior:
		// オフセット
		offset_ = LwLib::GetRandomValue({ -offsetValue,-offsetValue,-offsetValue }, { offsetValue,offsetValue,offsetValue }, limit);

		break;
	case TrackingType::kSuperior:

		break;
	default:
		break;
	}

}

void TrackingBullet::TrackUpdate()
{
	// プレイヤーがいない場合早期
	if (!object_) {
		return;
	}
	// 追尾の計算
	switch (trackingType_)
	{
		// 通常
	case TrackingType::kStandard:
		CalcStandardMissile();
		break;
		// 優等
	case TrackingType::kInferior:
		CalcInferiorMissile();
		break;
		// 劣等
	case TrackingType::kSuperior:
		CalcSuperiorMissile();
		break;
	default:
		break;
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

void TrackingBullet::CalcStandardMissile()
{
	// それぞれのベクトル
	Vector3 toTarget = object_->worldTransform_.GetWorldPosition() - GetWorldPosition();
	Vector3 nowDirect = Vector3::Normalize(velocity_);
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
	float maxCentripetalForce = std::powf(TrackingBullet::sBaseVelocity, 2) / sLerpRadius;

	// 力の向き
	Vector3 force = centripetalAccel * maxCentripetalForce;
	// 推進力計算
	float propulsion = TrackingBullet::sBaseVelocity * TrackingBullet::sDamping;
	// 向心力に現在の方向ベクトルに＋推進力でベクトルを作成
	force += nowDirect * propulsion;
	// 速度の減衰処理
	force -= velocity_ * TrackingBullet::sDamping;
	accelerate_ = force * GameSystem::GameSpeedFactor();
}

void TrackingBullet::CalcInferiorMissile()
{
	// それぞれのベクトル
	Vector3 playerPos = object_->worldTransform_.GetWorldPosition() + offset_;
	Vector3 toTarget = playerPos - GetWorldPosition();
	Vector3 nowDirect = Vector3::Normalize(velocity_);
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
	float maxCentripetalForce = std::powf(TrackingBullet::sBaseVelocity, 2) / sLerpRadius;
	// 力の向き
	Vector3 force = centripetalAccel * maxCentripetalForce;
	// 推進力計算
	float propulsion = TrackingBullet::sBaseVelocity * TrackingBullet::sDamping;
	// 向心力に現在の方向ベクトルに＋推進力でベクトルを作成
	force += nowDirect * propulsion;
	// 速度の減衰処理
	force -= velocity_ * TrackingBullet::sDamping;
	// 速度処理
	accelerate_ = force * GameSystem::GameSpeedFactor();

}

void TrackingBullet::CalcSuperiorMissile()
{
	// プレイヤーの現在の位置と速度
	Vector3 playerPosition = object_->worldTransform_.GetWorldPosition();
	Vector3 playerDirection = object_->worldTransform_.GetWorldPosition() - object_->prevPosition_;
	Vector3 predictedPosition{};	// 予測先の座標
	// 予測位置を計算
	float predictionTime = GlobalVariables::GetInstance()->GetValue<float>("TrackSuperior", "PredictionTime"); // ミサイルが向かう予測時間
	// 予測先の計算
	if (playerDirection.x == 0.0f && playerDirection.y == 0.0f && playerDirection.z == 0.0f) {
		predictedPosition = playerPosition;
	}
	else {
		predictedPosition = playerPosition + (playerDirection * predictionTime);
	}

	Vector3 toTarget = predictedPosition - GetWorldPosition();
	Vector3 nowDirect = Vector3::Normalize(velocity_);
	float dot = Vector3::Dot(toTarget, nowDirect);

	Vector3 centripetalAccel = toTarget - (nowDirect * dot);
	float centripetalAccelMagnitude = Vector3::Length(centripetalAccel);

	if (centripetalAccelMagnitude > 2.0f) {
		centripetalAccel /= centripetalAccelMagnitude;
	}

	float maxCentripetalForce = std::powf(TrackingBullet::sBaseVelocity, 2) / sLerpRadius;

	Vector3 force = centripetalAccel * maxCentripetalForce;

	float propulsion = TrackingBullet::sBaseVelocity * TrackingBullet::sDamping;

	force += nowDirect * propulsion;
	force -= velocity_ * TrackingBullet::sDamping;

	accelerate_ = force * GameSystem::GameSpeedFactor();
}
