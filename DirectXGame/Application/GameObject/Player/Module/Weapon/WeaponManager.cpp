#include "WeaponManager.h"
#include "Application/GameObject/GameObjectLists.h"
#include "Application/GameSystem/GameSystem.h"

void WeaponManager::Initialize()
{
	weapon_ = std::make_unique<IWeapon>();
	weapon_->Initialize(player_);
	weapon_->worldTransform_.transform_.translate.y = 2.0f;

	lockOn_.Initialize(player_);
	aimManager_.Initialize(player_);
	//weapon_->worldTransform_.transform_.translate.x = 5.0f;
}

void WeaponManager::Update()
{
	shotDuration_.Update(GameSystem::sSpeedFactor);
	lockOnDuration_.Update(GameSystem::sSpeedFactor);

	weapon_->Update();

	lockOn_.Update();
	aimManager_.Update(player_->GetCamera());
}

void WeaponManager::AttackReception()
{
	// クールタイム
	if (shotDuration_.IsActive()) {
		return;
	}

	Vector3 velocity = Vector3::Normalize(aimManager_.GetWorldPosition() - player_->worldTransform_.GetWorldPosition());
	player_->GetSystemFacede()->GetShootingManager()->OnFire(velocity);
	shotDuration_.Start(GlobalVariables::GetInstance()->GetValue<float>("Player", "ShotDuration"));

}

void WeaponManager::LockOnReception()
{
	// クールタイム
	if (lockOnDuration_.IsActive()) {
		return;
	}

	lockOn_.ToggleLockOn(player_->GetCamera());
	lockOnDuration_.Start(GlobalVariables::GetInstance()->GetValue<float>("Player", "LockDuration"));

}
