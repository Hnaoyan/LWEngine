#include "Boss.h"
#include "imgui.h"
#include "Application/GameObject/GameObjectLists.h"
#include "Engine/LwLib/LwEngineLib.h"
#include "Engine/3D/ModelUtility/ModelManager.h"
#include "Engine/3D/ModelUtility/ModelRenderer.h"
#include "Engine/GlobalVariables/GlobalVariables.h"
#include "Engine/Particle/GPUParticleSystem.h"

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
	animationManager_ = std::make_unique<BossSystemContext::AnimationManager>();
	animationManager_->Initialize(this);

	// ステートマネージャー
	stateManager_.Initialize(this);
	stateManager_.ChangeRequest(std::make_unique<BossState::WaitState>());

	bulletManager_ = std::make_unique<BossSystemContext::BulletManager>();
	bulletManager_->SetGPUParticle(gpuParticle_);
	bulletManager_->Initialize(ModelManager::GetModel("DefaultCube"), this);

	respawnPos_ = { 0,8.5f,50.0f };

	worldTransform_.transform_.translate = respawnPos_;
	collider_.Initialize(worldTransform_.transform_.scale.x, this);
	collider_.SetAttribute(kCollisionAttributeEnemy);
	isAction_ = true;
}

void Boss::Update()
{
	// システム
	systemManager_->Update();
	// アニメ
	animationManager_->Update();
	// 弾
	bulletManager_->Update();
	// ステート
	if (state_ && isAction_) {
		state_->Update();

	}
	// 座標更新
	IGameObject::Update();
	// バリア時の当たり判定
	if (systemManager_->barrierManager_.IsActive()) {
		collider_.radius_ = GlobalVariables::GetInstance()->GetValue<Vector3>("Boss", "BarrierScale").x;
	}
	else {
		collider_.radius_ = GlobalVariables::GetInstance()->GetValue<Vector3>("Boss", "NormalScale").x;
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
	//---描画---//
	if (!isInvisible_) {
		// アニメーション
		animationManager_->Draw(desc.camera, lightDesc);
		// バリア
		systemManager_->barrierManager_.Draw(desc.camera, lightDesc);
	}
	// 弾
	bulletManager_->Draw(desc);
}

void Boss::ImGuiDraw()
{
	
	ImGui::Begin("Boss");
	if (ImGui::Button("MissileState")) {
		stateManager_.ChangeRequest(std::make_unique<BossState::MissileAttackState>());
	}
	ImGui::Checkbox("IsAction", &isAction_);
	ImGui::Checkbox("IsInvisible", &isInvisible_);
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
		if (ImGui::BeginTabItem("Anim")) {
			if (ImGui::Button("Open")) {
				animationManager_->AnimationExecute(BossSystemContext::AnimationManager::AnimType::kOpen, 60.0f);
			}
			if (ImGui::Button("Close")) {
				animationManager_->AnimationExecute(BossSystemContext::AnimationManager::AnimType::kClose, 60.0f);
			}
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}

	systemManager_->ImGuiDraw();

	ImGui::End();

}

void Boss::OnCollision(ColliderObject target)
{
	if (std::holds_alternative<IBullet*>(target)) {
		IBullet** bullet = std::get_if<IBullet*>(&target);
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
			float damageRatio = 1.0f;
			if (animationManager_->IsOpen()) {
				damageRatio *= 2.5f;
			}
			// 距離に応じて
			if (distance >= 150.0f) {
				systemManager_->healthManager_.TakeDamage(damageRatio * 0.25f);
			}
			if (distance >= 100.0f) {
				systemManager_->healthManager_.TakeDamage(damageRatio * 0.4f);
			}
			else if (distance >= 75.0f) {
				systemManager_->healthManager_.TakeDamage(damageRatio * 0.5f);
			}
			else {
				systemManager_->healthManager_.TakeDamage(damageRatio);
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

void Boss::Finalize()
{
	gpuParticle_->DeleteEmitter("BossDamage");
}

void Boss::GlobalValueInitialize()
{
	GlobalVariables* instance = GlobalVariables::GetInstance();
	std::string groupName = "Boss";
	//---ボスの共通---//
	instance->CreateGroup(groupName);
	instance->AddValue(groupName, "NormalScale", worldTransform_.transform_.scale);
	instance->AddValue(groupName, "BarrierScale", worldTransform_.transform_.scale * 2.0f);
	instance->AddValue(groupName, "BarrierHP", 4.0f);
	instance->AddValue(groupName, "BarrierVanishFrame", 45.0f);
	instance->AddValue(groupName, "BarrierReappearFrame", 30.0f);

	//---通常弾---//
	groupName = "BossNormalBullet";
	instance->CreateGroup(groupName);
	instance->AddValue(groupName, "Acceleration", BossSystemContext::NormalBullet::sAcceleration);
	instance->AddValue(groupName, "Scale", Vector3(1.0f, 1.0f, 1.0f));

	//---追尾弾---//
	groupName = "BossTrackingBullet";
	instance->CreateGroup(groupName);
	instance->AddValue(groupName, "TrackFrame", BossSystemContext::TrackingBullet::sTrackingFrame);
	instance->AddValue(groupName, "Damping", BossSystemContext::TrackingBullet::sDamping);
	instance->AddValue(groupName, "Speed", BossSystemContext::TrackingBullet::sBulletSpeed);
	instance->AddValue(groupName, "InitSpeed", BossSystemContext::TrackingBullet::sInitSpeed);
	instance->AddValue(groupName, "LerpRadius", BossSystemContext::TrackingBullet::sLerpRadius);
	instance->AddValue(groupName, "Scale", Vector3(1.0f, 1.0f, 1.0f));
	instance->AddValue(groupName, "TrailSaveFrame", int32_t(50));
	instance->AddValue(groupName, "TrailMaxWidth", float(1.0f));
	instance->AddValue(groupName, "TrailMinWidth", float(0.25f));
}
