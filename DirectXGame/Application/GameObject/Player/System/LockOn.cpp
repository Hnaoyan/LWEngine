#include "LockOn.h"
#include "../../GameObjectLists.h"
#include "Engine/Math/MathLib.h"
#include <cassert>

void LockOn::Initialize(Player* player)
{
	assert(player);
	player_ = player;
}

void LockOn::ToggleLockOn(ICamera* camera)
{
	// ターゲット処理
	if (target_ == nullptr) {
		SearchTarget(camera);
	}
	// 解除処理
	else {
		TargetRelease();
	}
}

void LockOn::TargetRelease()
{
	// 対象をnullに
	target_ = nullptr;
}

void LockOn::SearchTarget(ICamera* camera)
{
	for (std::vector<std::unique_ptr<SampleEnemy>>::iterator it = enemys_->begin();
		it != enemys_->end(); ++it) {

		Vector3 cameraToEnemy = (*it)->worldTransform_.transform_.translate - player_->camera_->transform_.translate;
		Vector3 cameraToPlayer = player_->worldTransform_.GetWorldPosition() - player_->camera_->transform_.translate;

		float dot = Vector3::Dot(Vector3::Normalize(cameraToEnemy), Vector3::Normalize(cameraToPlayer));

		if (dot > 0.0f) {

		}

	}
	camera;

}
