#include "Boss.h"
#include "imgui.h"
#include "Application/GameObject/GameObjectLists.h"
#include "Engine/LwLib/LwEngineLib.h"
#include "Engine/3D/ModelManager.h"
#include "Engine/GlobalVariables/GlobalVariables.h"

void Boss::Initialize(Model* model)
{
#ifdef _DEBUG
	GlobalValueInitialize();
#endif // _DEBUG

	IGameObject::Initialize(model);
	// ステートマネージャー
	stateManager_.Initialize(this);
	stateManager_.ChangeRequest(std::make_unique<BossState::WaitState>());

	bulletManager_ = std::make_unique<BossSystemContext::BulletManager>();
	bulletManager_->SetGPUParticle(gpuParticle_);
	bulletManager_->Initialize(ModelManager::GetModel("DefaultCube"), this);

	healthManager_.Initialize(20);

	particleManager_.Initialize(this);

	uiManager_.Initialize(this);

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
	particleManager_.Update();
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
	Vector2 boss = { worldTransform_.GetWorldPosition().x,worldTransform_.GetWorldPosition().z };
	Vector2 player = { player_->worldTransform_.GetWorldPosition().x,player_->worldTransform_.GetWorldPosition().z };
	float distance = Vector2::Distance(boss, player);
	ImGui::DragFloat("PlayerDistance", &distance);

	// システムのタブ
	if (ImGui::BeginTabBar("BulletInfo"))
	{
		// 通常弾
		if (ImGui::BeginTabItem("Normal")) {
			ImGui::DragFloat("N_Acceleration", &BossSystemContext::NormalBullet::sAcceleration, 0.01f);
			ImGui::EndTabItem();
		}
		// 追尾弾
		if (ImGui::BeginTabItem("Tracking")) {
			ImGui::DragFloat("T_TrackingFrame", &BossSystemContext::TrackingBullet::sTrackingFrame, 0.01f);
			ImGui::DragFloat("T_BulletSpeed", &BossSystemContext::TrackingBullet::sBulletSpeed, 0.01f);
			ImGui::DragFloat("T_InitSpeed", &BossSystemContext::TrackingBullet::sInitSpeed, 0.01f);
			ImGui::DragFloat("T_Damping", &BossSystemContext::TrackingBullet::sDamping, 0.01f);
			ImGui::DragFloat("T_LerpRadius", &BossSystemContext::TrackingBullet::sLerpRadius, 0.01f);
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}

	if (ImGui::BeginTabBar("System"))
	{
		// 通常弾
		if (ImGui::BeginTabItem("UI")) {
			uiManager_.ImGuiDraw();
			ImGui::EndTabItem();
		}
		// 追尾弾
		if (ImGui::BeginTabItem("Test")) {

			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}


	ImGui::End();

}

void Boss::OnCollision(ColliderObject target)
{
	if (std::holds_alternative<IBullet*>(target)) {
		IBullet** bullet = std::get_if<IBullet*>(&target);
		//float lifeTime = (*bullet)->GetLifeTime();
		//if (lifeTime >= 1.0f) {
		//	healthManager_.TakeDamage(1.0f * 0.5f);
		//}
		//else {
		//	healthManager_.TakeDamage();
		//}

		Vector2 xzBullet = { (*bullet)->GetGeneratePosition().x,(*bullet)->GetGeneratePosition().z };
		Vector2 xzBoss = { worldTransform_.GetWorldPosition().x ,worldTransform_.GetWorldPosition().z };
		float distance = Vector2::Distance(xzBoss, xzBullet);

		if (distance >= 150.0f) {
			healthManager_.TakeDamage(1.0f * 0.25f);
		}
		if (distance >= 100.0f) {
			healthManager_.TakeDamage(1.0f * 0.4f);
		}
		else if (distance >= 75.0f) {
			healthManager_.TakeDamage(1.0f * 0.5f);
		}
		else {
			healthManager_.TakeDamage(1.0f);
		}

		particleManager_.OnBulletHit();
		if (healthManager_.IsDead()) {
			isDead_ = true;
		}
	}
}

void Boss::UIDraw()
{
	this->uiManager_.Draw();
}

void Boss::GlobalValueInitialize()
{
	GlobalVariables* instance = GlobalVariables::GetInstance();
	instance->CreateGroup("BossNormalBullet");
	instance->AddValue("BossNormalBullet", "Acceleration", BossSystemContext::NormalBullet::sAcceleration);
	instance->CreateGroup("BossTrackingBullet");
	instance->AddValue("BossTrackingBullet", "TrackFrame", BossSystemContext::TrackingBullet::sTrackingFrame);
	instance->AddValue("BossTrackingBullet", "Damping", BossSystemContext::TrackingBullet::sDamping);
	instance->AddValue("BossTrackingBullet", "Speed", BossSystemContext::TrackingBullet::sBulletSpeed);
	instance->AddValue("BossTrackingBullet", "InitSpeed", BossSystemContext::TrackingBullet::sInitSpeed);
	instance->AddValue("BossTrackingBullet", "LerpRadius", BossSystemContext::TrackingBullet::sLerpRadius);
}
