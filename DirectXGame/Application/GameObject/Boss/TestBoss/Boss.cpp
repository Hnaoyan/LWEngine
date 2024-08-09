#include "Boss.h"
#include "imgui.h"
#include "Engine/3D/ModelManager.h"

void Boss::Initialize(Model* model)
{
	IGameObject::Initialize(model);
	// ステートマネージャー
	stateManager_.Initialize(this);
	stateManager_.ChangeRequest(std::make_unique<BossState::MoveState>());

	bulletManager_ = std::make_unique<BossSystemContext::BulletManager>();
	bulletManager_->Initialize(ModelManager::GetModel("DefaultCube"));

	healthManager_.Initialize(20);

	respawnPos_ = { 0,20.0f,50.0f };

	worldTransform_.transform_.translate = respawnPos_;
	worldTransform_.transform_.scale = { 7.5f,7.5f,7.5f };

	collider_.Initialize(worldTransform_.transform_.scale.x, this);
	collider_.SetAttribute(kCollisionAttributeEnemy);
}

void Boss::Update()
{
	bulletManager_->Update();

	if (state_) {
		state_->Update();
	}
	// 座標更新
	IGameObject::Update();
	collider_.Update(worldTransform_.GetWorldPosition());
}

void Boss::Draw(ModelDrawDesc desc)
{
	ModelDrawDesc drawDesc{};
	drawDesc.camera = desc.camera;
	drawDesc.directionalLight = desc.directionalLight;
	drawDesc.pointLight = desc.pointLight;
	drawDesc.spotLight = desc.spotLight;
	drawDesc.worldTransform = &worldTransform_;

	model_->Draw(drawDesc);
	bulletManager_->Draw(desc);
}

void Boss::ImGuiDraw()
{
	
	ImGui::Begin("Boss");
	ImGui::DragFloat3("Scale", &worldTransform_.transform_.scale.x, 0.01f);
	collider_.radius_ = worldTransform_.transform_.scale.x;
	ImGui::End();

}

void Boss::OnCollision(ColliderObject target)
{
	if (std::holds_alternative<IBullet*>(target)) {
		healthManager_.TakeDamage();
		if (healthManager_.IsDead()) {
			isDead_ = true;
		}
	}
}