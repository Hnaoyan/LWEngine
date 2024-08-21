#include "Boss.h"
#include "imgui.h"
#include "Application/GameObject/GameObjectLists.h"
#include "Engine/LwLib/LwEngineLib.h"
#include "Engine/3D/ModelManager.h"

void Boss::Initialize(Model* model)
{
	IGameObject::Initialize(model);
	// ステートマネージャー
	stateManager_.Initialize(this);
	stateManager_.ChangeRequest(std::make_unique<BossState::WaitState>());

	bulletManager_ = std::make_unique<BossSystemContext::BulletManager>();
	bulletManager_->SetGPUParticle(gpuParticle_);
	bulletManager_->Initialize(ModelManager::GetModel("DefaultCube"));

	healthManager_.Initialize(20);

	particleManager_.Initialize(this);

	respawnPos_ = { 0,8.5f,50.0f };

	worldTransform_.transform_.translate = respawnPos_;
	worldTransform_.transform_.scale = { 7.5f,7.5f,7.5f };
	curveTime_.Start(300.0f);
	collider_.Initialize(worldTransform_.transform_.scale.x, this);
	collider_.SetAttribute(kCollisionAttributeEnemy);
}

void Boss::Update()
{
	bulletManager_->Update();
	curveTime_.Update();
	if (state_) {
		state_->Update();
		//Vector3 Lfront = { -50.0f,8.5f,-50.0f };
		//Vector3 Lback = { -50.0f,8.5f,50.0f };
		//Vector3 Rfront{ 50.0f,8.5f,-50.0f };
		//Vector3 Rback{ 50.0f,8.5f,50.0f };
		//Vector2 xtoz{};
		//if (!isHalf_) {
		//	xtoz = LwLib::Curve::BezierCurve({ 0.0f,50.0f }, { Rback.x,Rback.z }, { 50.0f,0.0f }, curveTime_.GetElapsedFrame());
		//}
		//else {
		//	xtoz = LwLib::Curve::BezierCurve({ 50.0f,0.0f }, { Rfront.x,Rfront.z }, { 0.0f,-50.0f }, curveTime_.GetElapsedFrame());
		//}
		//if (curveTime_.IsEnd() && !isHalf_) {
		//	isHalf_ = true;
		//	curveTime_.Start(150.0f);
		//}
		//worldTransform_.transform_.translate.x = xtoz.x;
		//worldTransform_.transform_.translate.z = xtoz.y;

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
