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
	transform.translate = player_->worldTransform_.GetWorldPosition();
	float speed = 300.0f;
	bulletManager_->FindCluster("NormalBullet")->AddBullet(transform, direct * speed);
}
