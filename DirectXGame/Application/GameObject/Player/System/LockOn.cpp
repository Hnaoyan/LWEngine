#include "LockOn.h"
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

	camera;

}
