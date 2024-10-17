#include "TrackingBullet.h"
#include "Application/Collision/ColliderFilter.h"
#include "Application/GameSystem/GameSystem.h"
#include "Application/GameObject/GameObjectLists.h"
#include "Engine/LwLib/LwEngineLib.h"
#include "Engine/GlobalVariables/GlobalVariables.h"

float TrackingBullet::sTrackingFrame = 95.0f;
float TrackingBullet::sDamping = 0.1f;
float TrackingBullet::sBulletSpeed = 90.0f;
float TrackingBullet::sInitSpeed = 70.0f;
float TrackingBullet::sLerpRadius = 50.0f;

void TrackingBullet::Initialize()
{
	// 基底の初期化・コライダーのマスク設定
	IBullet::Initialize();
	collider_.SetAttribute(kCollisionAttributeEnemyBullet);

	GlobalVariables* instance = GlobalVariables::GetInstance();
	sTrackingFrame = instance->GetValue<float>("BossTrackingBullet", "TrackFrame");
	sDamping = instance->GetValue<float>("BossTrackingBullet", "Damping");
	sBulletSpeed = instance->GetValue<float>("BossTrackingBullet", "Speed");
	sInitSpeed = instance->GetValue<float>("BossTrackingBullet", "InitSpeed");
	sLerpRadius = instance->GetValue<float>("BossTrackingBullet", "LerpRadius");
	
	// 直進の時間設定
	straightTimer_.Start(instance->GetValue<float>("BossTrackingBullet", "StraightFrame"));
	// タイプごとの初期化
	SetupByType();
	// 軌跡
	trail_->SetLength(50);
	trail_->polygon_->SetColor({ 1.0f,1.0f,1.0f,1.0f });
	trail_->polygon_->SetWidth(instance->GetValue<float>("BossTrackingBullet", "TrailMaxWidth"));
	trail_->polygon_->SetMinWidth(instance->GetValue<float>("BossTrackingBullet", "TrailMinWidth"));
}

void TrackingBullet::Update()
{
	// 追尾タイマー
	straightTimer_.Update();
	trackTimer_.Update();

	if (straightTimer_.IsEnd()) {
		trackTimer_.Start(TrackingBullet::sTrackingFrame);
	}
		
	// 追尾の速度計算処理
	TrackUpdate();

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
	if (trackTimer_.IsActive()) {
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
	float maxCentripetalAccel = std::powf(TrackingBullet::sBulletSpeed, 2) / sLerpRadius;

	// 力の向き
	Vector3 force = centripetalAccel * maxCentripetalAccel;
	// 推進力計算
	float propulsion = TrackingBullet::sBulletSpeed * TrackingBullet::sDamping;
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
	float maxCentripetalAccel = std::powf(TrackingBullet::sBulletSpeed, 2) / sLerpRadius;
	// 力の向き
	Vector3 force = centripetalAccel * maxCentripetalAccel;
	// 推進力計算
	float propulsion = TrackingBullet::sBulletSpeed * TrackingBullet::sDamping;
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

	float maxCentripetalAccel = std::powf(TrackingBullet::sBulletSpeed, 2) / sLerpRadius;

	Vector3 force = centripetalAccel * maxCentripetalAccel;

	float propulsion = TrackingBullet::sBulletSpeed * TrackingBullet::sDamping;

	force += nowDirect * propulsion;
	force -= velocity_ * TrackingBullet::sDamping;

	accelerate_ = force * GameSystem::GameSpeedFactor();
}
