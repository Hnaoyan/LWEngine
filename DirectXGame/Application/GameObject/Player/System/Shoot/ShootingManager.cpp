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
	bulletManager_->FindCluster("PlayerNormalBullet")->AddBullet(std::move(bullet));
}
