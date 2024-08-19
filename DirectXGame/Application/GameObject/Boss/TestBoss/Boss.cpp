#include "Boss.h"
#include "imgui.h"
#include "Application/GameObject/GameObjectLists.h"
#include "Engine/3D/ModelManager.h"

void Boss::Initialize(Model* model)
{
	IGameObject::Initialize(model);
	// ステートマネージャー
	stateManager_.Initialize(this);
	stateManager_.ChangeRequest(std::make_unique<BossState::WaitState>());

	bulletManager_ = std::make_unique<BossSystemContext::BulletManager>();
	bulletManager_->Initialize(ModelManager::GetModel("DefaultCube"));

	healthManager_.Initialize(20);

	respawnPos_ = { 0,8.5f,50.0f };

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
		Vector3 normalize = player_->worldTransform_.GetWorldPosition() - worldTransform_.GetWorldPosition();
		normalize = Vector3::Normalize(normalize);
		normalize *= -1.0f;
		worldTransform_.transform_.rotate.y = std::atan2f(normalize.x, normalize.z);
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
	ImGui::DragFloat3("Position", &worldTransform_.transform_.translate.x, 0.1f);
	ImGui::DragFloat3("Rotate", &worldTransform_.transform_.rotate.x, 0.01f);
	ImGui::DragFloat3("Scale", &worldTransform_.transform_.scale.x, 0.01f);
	collider_.radius_ = worldTransform_.transform_.scale.x;
	float distance = Vector3::Distance(worldTransform_.GetWorldPosition(), player_->worldTransform_.GetWorldPosition());
	ImGui::DragFloat("PlayerDistance", &distance);

	ImGui::DragFloat("TrackingFrame", &BossSystemContext::TrackingBullet::sTrackingFrame, 0.01f);
	ImGui::DragFloat("BulletSpeed", &BossSystemContext::TrackingBullet::sBulletSpeed, 0.01f);
	ImGui::DragFloat("Damping", &BossSystemContext::TrackingBullet::sDamping, 0.01f);
	ImGui::DragFloat("LerpRadius", &BossSystemContext::TrackingBullet::sLerpRadius, 0.01f);

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
