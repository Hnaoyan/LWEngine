#include "BulletBombEffect.h"
#include "Application/GameSystem/GameSystem.h"

void BulletBombEffect::Initialize()
{
	// 基底初期化
	InstancedUnit::Initialize();
	deleteTimer_.Start(30.0f);
	transform_.scale = { 5.0f,5.0f,1.0f };
	defaultScale_ = transform_.scale;
	goalScale_ = transform_.scale / 2.0f;
	isBillBoard_ = true;
}

void BulletBombEffect::Update()
{
	// 移動
	//transform_.translate += velocity_ * GameSystem::GameSpeedFactor();

	// タイマー更新
	deleteTimer_.Update();
	if (deleteTimer_.IsActive()) {
		this->transform_.scale = Ease::Easing(Vector3(defaultScale_), Vector3(goalScale_), deleteTimer_.GetElapsedFrame());
	}

	// 削除処理
	if (deleteTimer_.IsEnd()) {
		isDead_ = true;
		return;
	}

	InstancedUnit::Update();
}
