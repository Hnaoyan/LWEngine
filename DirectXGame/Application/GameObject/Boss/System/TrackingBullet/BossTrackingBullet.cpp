#include "../BossSystem.h"
#include "Application/Collision/ColliderFilter.h"
#include "Application/GameSystem/GameSystem.h"
#include "Application/GameObject/GameObjectLists.h"
#include "Engine/LwLib/LwEngineLib.h"

float BossSystemContext::TrackingBullet::sTrackingFrame = 95.0f;
float BossSystemContext::TrackingBullet::sDamping = 0.1f;
float BossSystemContext::TrackingBullet::sBulletSpeed = 90.0f;
float BossSystemContext::TrackingBullet::sInitSpeed = 70.0f;
float BossSystemContext::TrackingBullet::sLerpRadius = 50.0f;


void BossSystemContext::TrackingBullet::Initialize()
{
	GlobalVariables* instance = GlobalVariables::GetInstance();
	sTrackingFrame = instance->GetValue<float>("BossTrackingBullet", "TrackFrame");
	sDamping = instance->GetValue<float>("BossTrackingBullet", "Damping");
	sBulletSpeed = instance->GetValue<float>("BossTrackingBullet", "Speed");
	sInitSpeed = instance->GetValue<float>("BossTrackingBullet", "InitSpeed");
	sLerpRadius = instance->GetValue<float>("BossTrackingBullet", "LerpRadius");

	serialNumber_ = sSerialNumber;
	sSerialNumber++;

	tag_ = "BossBullet" + std::to_string(serialNumber_);

	InstancedUnit::Initialize();
	transform_ = { {1.0f,1.0f,1.0f} ,{0,0,0},{0.0f,0.0f,0.0f} };
	//transform_.scale *= 3.5f;
	collider_.Initialize(transform_.scale.x, this);
	collider_.SetAttribute(kCollisionAttributeEnemyBullet);

	trackTimer_.Start(TrackingBullet::sTrackingFrame);

	trackType_ = TrackType::kInferior;

}

void BossSystemContext::TrackingBullet::Update()
{

	trackTimer_.Update();

	if (trackTimer_.IsActive()) {
		switch (trackType_)
		{
		case BossSystemContext::TrackingBullet::TrackType::kStandard:
			CalcStandardMissile();
			break;
		case BossSystemContext::TrackingBullet::TrackType::kInferior:
			CalcInferiorMissile();
			break;
		case BossSystemContext::TrackingBullet::TrackType::kSuperior:
			CalcSuperiorMissile();
			break;
		default:
			break;
		}
	}

	// 移動
	transform_.translate += velocity_ * GameSystem::GameSpeedFactor();

	InstancedUnit::Update();
	collider_.Update(transform_.translate);
}

void BossSystemContext::TrackingBullet::ImGuiDraw()
{
}

void BossSystemContext::TrackingBullet::OnCollision(ColliderObject object)
{
	if (std::holds_alternative<Terrain*>(object)) {
		isDead_ = true;
	}
	if (std::holds_alternative<Player*>(object)) {
		isDead_ = true;
	}
}

void BossSystemContext::TrackingBullet::CalcStandardMissile()
{
	// それぞれのベクトル
	Vector3 toTarget = player_->worldTransform_.GetWorldPosition() - GetWorldPosition();
	Vector3 nowDirect = Vector3::Normalize(velocity_);
	// 内積
	float dot = Vector3::Dot(toTarget, nowDirect);

	// 向心加速力の計算
	Vector3 centripetalAccel = toTarget - (nowDirect * dot);
	float centripetalAccelMagnitude = Vector3::Length(centripetalAccel);
	// 大きさの調整
	if (centripetalAccelMagnitude > 1.0f) {
		centripetalAccel /= centripetalAccelMagnitude;
	}
	// 最大向心力
	float maxCentripetalAccel = std::powf(TrackingBullet::sBulletSpeed, 2) / TrackingBullet::sLerpRadius;
	// ずらすオフセット作成
	float offsetValue = 4.0f;
	Vector3 offset = LwLib::GetRandomValue({ -offsetValue,-offsetValue,-offsetValue }, { offsetValue,offsetValue,offsetValue });
	centripetalAccel += offset;
	
	// 力の向き
	Vector3 force = centripetalAccel * maxCentripetalAccel;
	// 推進力計算
	float propulsion = TrackingBullet::sBulletSpeed * TrackingBullet::sDamping;
	// 向心力に現在の方向ベクトルに＋推進力でベクトルを作成
	force += nowDirect * propulsion;
	// 速度の減衰処理
	force -= velocity_ * TrackingBullet::sDamping;

	velocity_ += force * GameSystem::GameSpeedFactor();
}

void BossSystemContext::TrackingBullet::CalcInferiorMissile()
{
	// それぞれのベクトル
	Vector3 toTarget = player_->worldTransform_.GetWorldPosition() - GetWorldPosition();
	Vector3 nowDirect = Vector3::Normalize(velocity_);
	// 内積
	float dot = Vector3::Dot(toTarget, nowDirect);

	// 向心加速力の計算
	Vector3 centripetalAccel = toTarget - (nowDirect * dot);
	float centripetalAccelMagnitude = Vector3::Length(centripetalAccel);
	// 大きさの調整
	if (centripetalAccelMagnitude > 1.0f) {
		centripetalAccel /= centripetalAccelMagnitude;
	}
	// 最大向心力
	float maxCentripetalAccel = std::powf(TrackingBullet::sBulletSpeed, 2) / TrackingBullet::sLerpRadius;
	// 力の向き
	Vector3 force = centripetalAccel * maxCentripetalAccel;
	// 推進力計算
	float propulsion = TrackingBullet::sBulletSpeed * TrackingBullet::sDamping;
	// 向心力に現在の方向ベクトルに＋推進力でベクトルを作成
	force += nowDirect * propulsion;
	// 速度の減衰処理
	force -= velocity_ * TrackingBullet::sDamping;

	velocity_ += force * GameSystem::GameSpeedFactor();

}

void BossSystemContext::TrackingBullet::CalcSuperiorMissile()
{
}
