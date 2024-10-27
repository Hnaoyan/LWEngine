#include "BulletBombEffect.h"
#include "Application/GameSystem/GameSystem.h"

void BulletBombEffect::Initialize()
{
	// 基底初期化
	InstancedUnit::Initialize();
	deleteTimer_.Start(30.0f);
	transform_.scale = { 5.0f,5.0f,1.0f };
	defaultScale_ = transform_.scale;
	isBillBoard_ = true;
}

void BulletBombEffect::Update()
{
	// 移動
	//transform_.translate += velocity_ * GameSystem::GameSpeedFactor();

	// タイマー更新
	deleteTimer_.Update();
	this->transform_.scale = Ease::Easing(defaultScale_, Vector3::Zero(), deleteTimer_.GetElapsedFrame());

	// 削除処理
	if (deleteTimer_.IsEnd()) {
		isDead_ = true;
		return;
	}

	InstancedUnit::Update();
}
