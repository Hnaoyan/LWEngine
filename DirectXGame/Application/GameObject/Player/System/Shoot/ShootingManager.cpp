#include "ShootingManager.h"
#include "Application/GameObject/GameObjectLists.h"

void PlayerContext::ShootingManager::Initialize(Player* player)
{
	player_ = player;

}

void PlayerContext::ShootingManager::Update()
{

}

void PlayerContext::ShootingManager::OnFire(const Vector3& direct)
{
	EulerTransform transform{};
	transform.scale = { 1.0f,1.0f,1.0f };
	transform.scale *= 0.5f;
	transform.translate = player_->worldTransform_.GetWorldPosition();
	float speed = 300.0f;
	std::unique_ptr<IBullet> bullet = std::make_unique<IBullet>();
	bullet->Initialize();
	bullet->SetVelocity(direct * speed);
	bullet->transform_ = transform;
	bullet->GetCollider()->SetAttribute(kCollisionAttributeBullet);
	bulletManager_->FindCluster("PlayerNormalBullet")->AddBullet(std::move(bullet));
}

void PlayerContext::ShootingManager::TrackingFire()
{
	GenerateTracking({ 1.0f,0.0f,0.0f }, TrackingType::kStandard);
	GenerateTracking({ -1.0f,0.0f,0.0f }, TrackingType::kStandard);
}

void PlayerContext::ShootingManager::GenerateTracking(const Vector3& direct, TrackingType type)
{
	Matrix4x4 rotateMatrix = Matrix4x4::MakeRotateXYZMatrix(player_->worldTransform_.transform_.rotate);
	EulerTransform transform = player_->worldTransform_.transform_;
	transform.scale *= 0.5f;

	// 回転行列と目標（初期方向
	Vector3 newDirect = Matrix4x4::TransformVector3(direct, rotateMatrix);
	float initSpeed = 30.0f;

	// 弾のインスタンス生成
	std::unique_ptr<IBullet> bullet = std::make_unique<TrackingBullet>();
	static_cast<TrackingBullet*>(bullet.get())->SetGameObject(player_->GetBoss());
	static_cast<TrackingBullet*>(bullet.get())->SetTrackType(type);
	bullet->Initialize();
	bullet->SetVelocity(newDirect * initSpeed);
	bullet->GetCollider()->SetAttribute(kCollisionAttributeBullet);
	bullet->transform_ = transform;
	bulletManager_->FindCluster("PlayerTrackingBullet")->AddBullet(std::move(bullet));
}
