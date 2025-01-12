#include "AreaLimiter.h"
#include "../../Player.h"

void PlayerContext::AreaLimiter::Initialize(Player* player)
{
	// 基底
	ISystem::Initialize(player);

	limitPoint_ = globalVariables_->GetValue<Vector3>("PlayerBasic", "LimitPosition");

}

void PlayerContext::AreaLimiter::Update()
{
	// X座標の制限
	if (player_->worldTransform_.GetWorldPosition().x >= limitPoint_.x) {
		player_->worldTransform_.transform_.translate.x = limitPoint_.x;
	}
	else if (player_->worldTransform_.GetWorldPosition().x <= -limitPoint_.x) {
		player_->worldTransform_.transform_.translate.x = -limitPoint_.x;
	}

	// Z座標の制限
	if (player_->worldTransform_.GetWorldPosition().z >= limitPoint_.z) {
		player_->worldTransform_.transform_.translate.z = limitPoint_.z;
	}
	else if (player_->worldTransform_.GetWorldPosition().z <= -limitPoint_.z) {
		player_->worldTransform_.transform_.translate.z = -limitPoint_.z;
	}

	// 自動死亡ライン
	if (player_->worldTransform_.GetWorldPosition().y <= -100.0f) {
		player_->SetIsDead(true);
	}

}
