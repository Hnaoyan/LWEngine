#include "ContainerBullet.h"
#include "Application/Collision/ColliderFilter.h"
#include "Application/GameSystem/GameSystem.h"
#include "Application/GameObject/GameObjectLists.h"
#include "Engine/LwLib/LwLibrary.h"

void ContainerBullet::Initialize()
{
	// 基底の初期化・コライダーのマスク設定
	IBullet::Initialize();
	collider_.SetAttribute(kCollisionAttributeEnemyBullet);
	const float fireDuration = 30.0f;	// 発射間隔の初期値
	fireTimer_.Start(fireDuration);
	
}

void ContainerBullet::Update()
{
	fireTimer_.Update();

	// 発射処理
	if (fireTimer_.IsEnd()) {
		GenerateBullet();
		const float fireDuration = 60.0f;
		fireTimer_.Start(fireDuration);
		deleteCount_++;
	}

	// 削除処理
	const int32_t maxCount = 5;
	if (deleteCount_ > maxCount) {
		isDead_ = true;
	}

	//// 速度の計算
	//accelerate_.x += sAcceleration * GameSystem::GameSpeedFactor();
	velocity_ += moveDirect_ * GameSystem::GameSpeedFactor();
	// 回転
	transform_.rotate.x += GameSystem::GameSpeedFactor();
	transform_.rotate.y += GameSystem::GameSpeedFactor();

	// 基底の更新
	IBullet::Update();
}

void ContainerBullet::OnCollision(ColliderObject object)
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

void ContainerBullet::GenerateBullet()
{
	Boss* boss = dynamic_cast<Boss*>(targetObject_);
	Player* player = dynamic_cast<Player*>(targetObject_);

	// 親がプレイヤーなら
	if (player) {
		// 設定部分
		EulerTransform transform = transform_;
		//float offsetValue = 2.0f;
		//transform.translate += Vector3::Normalize(Vector3(0.0f, 0.0f, -1.0f)) * offsetValue;
		// 生成部分
		BulletBuilder builder;
		builder.SetTargetObject(player->GetBoss()).SetDirect(Vector3::Backward()).SetSpeed(GlobalVariables::GetInstance()->GetValue<float>("BossTrackingBullet", "InitSpeed")).SetTransform(transform).SetAttribute(TrackingAttribute::kSuperior).SetIsRandStraight(true);
		builder.SetParentAttribute(1);
		cluster_->AddBullet(builder, BulletType::kTracking);
	}
	// 親がボスなら
	else if (boss) {
		// 設定部分
		EulerTransform transform = transform_;
		//float offsetValue = 2.0f;
		//transform.translate += Vector3::Normalize(Vector3(0.0f,0.0f,-1.0f)) * offsetValue;
		// 生成部分
		BulletBuilder builder;
		builder.SetTargetObject(boss->GetPlayer()).SetDirect(Vector3::Backward()).SetSpeed(GlobalVariables::GetInstance()->GetValue<float>("BossTrackingBullet", "InitSpeed")).SetTransform(transform).SetAttribute(TrackingAttribute::kSuperior).SetIsRandStraight(true);
		builder.SetParentAttribute(0);
		cluster_->AddBullet(builder, BulletType::kTracking);
	}

}
