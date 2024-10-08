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

	InstancedUnit::Initialize();
	transform_.scale *= 0.5f;
	collider_.Initialize(transform_.scale.x, this);
	collider_.SetAttribute(kCollisionAttributeBullet);
	// 生成座標
	generatePosition_ = transform_.translate;

	// 軌跡
	trail_ = std::make_unique<BulletTrail>();
	trail_->triangle_->SetColor({ 0.0f,1.0f,1.0f,1.0f });
	trail_->triangle_->SetWidth(0.75f);
	trail_->SetLength(10);
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
	// 軌跡の更新
	trail_->UpdateTrail(transform_.translate);
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

void IBullet::Draw(ICamera* camera)
{
	// 軌跡クラスの設定
	trail_->Draw(camera);
	// 軌跡の描画
	ModelRenderer::TriangleDraw(camera, trail_->triangle_.get());
}
