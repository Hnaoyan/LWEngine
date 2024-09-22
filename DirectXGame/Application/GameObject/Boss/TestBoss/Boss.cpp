#include "Boss.h"
#include "imgui.h"
#include "Application/GameObject/GameObjectLists.h"
#include "Engine/LwLib/LwEngineLib.h"
#include "Engine/3D/ModelManager.h"
#include "Engine/3D/ModelRenderer.h"
#include "Engine/GlobalVariables/GlobalVariables.h"

void Boss::Initialize(Model* model)
{
#ifdef _DEBUG
	GlobalValueInitialize();
#endif // _DEBUG

	IGameObject::Initialize(model);
	// システム
	systemManager_ = std::make_unique<BossFacade>();
	systemManager_->Initialize(this);

	// アニメーション
	animationManager_ = std::make_unique<BossAnimationManager>();
	animationManager_->Initialize(this);

	// ステートマネージャー
	stateManager_.Initialize(this);
	stateManager_.ChangeRequest(std::make_unique<BossState::WaitState>());

	bulletManager_ = std::make_unique<BossSystemContext::BulletManager>();
	bulletManager_->SetGPUParticle(gpuParticle_);
	bulletManager_->Initialize(ModelManager::GetModel("DefaultCube"), this);

	respawnPos_ = { 0,8.5f,50.0f };

	worldTransform_.transform_.translate = respawnPos_;
	worldTransform_.transform_.scale = { 7.5f,7.5f,7.5f };
	curveTime_.Start(300.0f);
	collider_.Initialize(worldTransform_.transform_.scale.x, this);
	collider_.SetAttribute(kCollisionAttributeEnemy);
}

void Boss::Update()
{
	// システム
	systemManager_->Update();
	// アニメ
	animationManager_->Update();
	// 弾
	bulletManager_->Update();
	// カーブ
	curveTime_.Update();
	// ステート
	if (state_) {
		state_->Update();

	}
	// 座標更新
	IGameObject::Update();
	// バリア時の当たり判定
	if (systemManager_->barrierManager_.IsActive()) {
		collider_.radius_ = worldTransform_.transform_.scale.x * 2.0f;
	}
	else {
		collider_.radius_ = worldTransform_.transform_.scale.x;
	}
	// コライダーの更新
	collider_.Update(worldTransform_.GetWorldPosition());
}

void Boss::Draw(ModelDrawDesc desc)
{
	// マテリアル更新
	model_->GetMaterial()->Update();
	// デスクの設定
	DrawDesc::LightDesc lightDesc{};
	DrawDesc::ModelDesc modelDesc{};
	lightDesc.directionalLight = desc.directionalLight;
	lightDesc.pointLight = desc.pointLight;
	lightDesc.spotLight = desc.spotLight;
	modelDesc.SetDesc(model_);
	modelDesc.worldTransform = &worldTransform_;
	// 描画
	//ModelRenderer::NormalDraw(desc.camera, modelDesc, lightDesc);
	// アニメーション
	animationManager_->Draw(desc.camera, lightDesc);
	// バリア
	systemManager_->barrierManager_.Draw(desc.camera, lightDesc);
	// 弾
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
			systemManager_->uiManager_.ImGuiDraw();
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
		// バリアとの衝突処理
		if (systemManager_->barrierManager_.IsActive()) {
			systemManager_->barrierManager_.DamageProcess(-1.0f);
			// バリアが割れる瞬間の処理
			if (systemManager_->barrierManager_.IsShattered()) {
				systemManager_->barrierManager_.BarrierBreak();
				stateManager_.ChangeRequest(std::make_unique<BossState::SystemDownState>());
			}
		}
		// 本体との衝突処理
		else {
			// 距離に応じて
			if (distance >= 150.0f) {
				systemManager_->healthManager_.TakeDamage(1.0f * 0.25f);
			}
			if (distance >= 100.0f) {
				systemManager_->healthManager_.TakeDamage(1.0f * 0.4f);
			}
			else if (distance >= 75.0f) {
				systemManager_->healthManager_.TakeDamage(1.0f * 0.5f);
			}
			else {
				systemManager_->healthManager_.TakeDamage(1.0f);
			}

			// オンヒットエフェクト
			systemManager_->particleManager_.OnBulletHit();
			if (systemManager_->healthManager_.IsDead()) {
				isDead_ = true;
			}
		}
	}
}

void Boss::UIDraw()
{
	systemManager_->uiManager_.Draw();
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
