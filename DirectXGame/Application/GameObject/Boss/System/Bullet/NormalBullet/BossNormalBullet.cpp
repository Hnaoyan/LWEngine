#include "../BossBullet.h"
#include "Application/Collision/ColliderFilter.h"
#include "Application/GameSystem/GameSystem.h"

float BossSystemContext::NormalBullet::sAcceleration = 0.1f;

void BossSystemContext::NormalBullet::Initialize()
{
	// 基底クラスの初期化・マスクの設定
	IBullet::Initialize();
	collider_.SetAttribute(kCollisionAttributeEnemyBullet);

	GlobalVariables* instance = GlobalVariables::GetInstance();
	sAcceleration = instance->GetValue<float>("BossNormalBullet", "Acceleration");
	acceleration_ = 0.0f;

	moveDirect_ = Vector3::Normalize(velocity_);

	// 軌跡の設定
	trail_->SetLength(50);
	trail_->polygon_->SetColor({ 1.0f,1.0f,1.0f,1.0f });

}

void BossSystemContext::NormalBullet::Update()
{
	// 速度の計算
	acceleration_ += sAcceleration * GameSystem::GameSpeedFactor();
	velocity_ += moveDirect_ * acceleration_;
	// 回転
	transform_.rotate.x += GameSystem::GameSpeedFactor() * 3.0f;
	transform_.rotate.y += GameSystem::GameSpeedFactor() * 2.0f;

	// 基底の更新
	IBullet::Update();
}

void BossSystemContext::NormalBullet::ImGuiDraw()
{

}

void BossSystemContext::NormalBullet::OnCollision(ColliderObject object)
{
	if (std::holds_alternative<Terrain*>(object)) {
		isDead_ = true;
	}
	if (std::holds_alternative<Player*>(object)) {
		isDead_ = true;
	}
}
