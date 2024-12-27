#include "WeaponManager.h"
#include "Application/GameObject/GameObjectLists.h"

void WeaponManager::Initialize()
{
	weapon_ = std::make_unique<IWeapon>();
	weapon_->Initialize(player_);
	weapon_->worldTransform_.transform_.translate.y = 2.0f;
	//weapon_->worldTransform_.transform_.translate.x = 5.0f;
}

void WeaponManager::Update()
{
	weapon_->Update();
}
