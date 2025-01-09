#include "BulletSpeedController.h"
#include "Engine/LwLib/LwEnginePaths.h"
#include "Application/GameSystem/GameSystem.h"

void BulletSpeedController::Initialize(const float& speed)
{
	// 初期値設定
	defaultSpeed_ = speed;
	currentSpeed_ = defaultSpeed_;
}

void BulletSpeedController::Update()
{
	currentframe_ += (1.0f / 60.0f);
}

float BulletSpeedController::GetNewSpeed()
{
	float offset = 50.0f;
	currentSpeed_ = LwLib::ExponentialInterpolate(currentSpeed_, currentSpeed_ + offset, 0.1f, GameSystem::GameSpeedFactor());
	//currentSpeed_ = Ease::Easing(defaultSpeed_, currentSpeed_ + offset, currentframe_);
	return currentSpeed_;
}
