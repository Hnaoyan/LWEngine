#include "IBullet.h"
#include "Application/Collision/ColliderFilter.h"
#include "Application/GameSystem/GameSystem.h"

#include "Engine/3D/ModelUtility/ModelRenderer.h"

#include <imgui.h>

uint32_t IBullet::sSerialNumber = 0;

void IBullet::Initialize()
{
	// シリアル番号
	serialNumber_ = sSerialNumber;
	sSerialNumber++;
	// タグ設定
	tag_ = "Bullet" + std::to_string(serialNumber_);

	InstancedUnit::Initialize();
	collider_.Initialize(transform_.scale.x, this);
	// 生成座標
	generatePosition_ = transform_.translate;

}

void IBullet::Update()
{
	// 生存時間
	lifeTime_ += GameSystem::GameSpeedFactor();
	// 移動
	transform_.translate += velocity_ * GameSystem::GameSpeedFactor();
	// サイズの設定
	collider_.radius_ = transform_.scale.x;
	collider_.Update(transform_.translate);
	// ユニットの基底
	InstancedUnit::Update();
}

void IBullet::ImGuiDraw()
{
	
}

void IBullet::OnCollision(ColliderObject object)
{
	if (std::holds_alternative<SampleEnemy*>(object)) {
		isDead_ = true;
	}
	if (std::holds_alternative<Boss*>(object)) {
		isDead_ = true;
	}
	if (std::holds_alternative<Terrain*>(object)) {
		isDead_ = true;
	}
}
