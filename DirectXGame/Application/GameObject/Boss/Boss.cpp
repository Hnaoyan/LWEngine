#include "Boss.h"
#include "imgui.h"
#include "Application/GameObject/GameObjectLists.h"
#include "Engine/LwLib/LwLibrary.h"
#include "Engine/Collision/CollisionManager.h"
#include "Engine/3D/ModelUtility/ModelManager.h"
#include "Engine/3D/ModelUtility/ModelRenderer.h"
#include "Engine/GlobalVariables/GlobalVariables.h"
#include "Engine/Particle/GPUParticleSystem.h"

void Boss::Initialize(Model* model)
{
	// debugなら
#ifdef IMGUI_ENABLED
	isAction_ = false;
#endif // IMGUI_ENABLED

	// Releaseなら
#ifdef RELEASE
	isAction_ = true;
#endif // RELEASE

	// 基底
	IGameObject::Initialize(model);
	// 丸影
	roundShadow_->SetDefaultScale(Vector2(30.0f, 30.0f));

	// システム
	systemManager_ = std::make_unique<BossFacade>();
	systemManager_->Initialize(this);

	// アニメーション
	animationManager_ = std::make_unique<BossSystemContext::AnimationManager>();
	animationManager_->Initialize(this);

	// ステートマネージャー
	stateManager_.Initialize(this);
	stateManager_.ChangeRequest(std::make_unique<BossState::WaitState>());

	// 座標関係の初期化
	worldTransform_.transform_.translate = GlobalVariables::GetInstance()->GetValue<Vector3>("Boss", "ResPosition");
	worldTransform_.transform_.translate.y = GlobalVariables::GetInstance()->GetValue<float>("Boss", "MinYPoint");
	float scale = GlobalVariables::GetInstance()->GetValue<float>("Boss", "ScaleRate");
	worldTransform_.transform_.scale = Vector3(scale, scale, scale);
	collider_.Initialize(worldTransform_.transform_.scale.x, this);
	collider_.SetAttribute(kCollisionAttributeEnemy);
}

void Boss::Update()
{
	// 1フレーム前の座標
	prevPosition_ = worldTransform_.GetWorldPosition();
	// システム
	systemManager_->Update();
	// ステート
	if (state_ && isAction_ && !isDead_) {
		state_->Update();

	}
	// 座標更新
	roundShadow_->SetPosition(Vector2(worldTransform_.GetWorldPosition().x, worldTransform_.GetWorldPosition().z));
	IGameObject::Update();

	// バリア時の当たり判定
	if (systemManager_->barrierManager_.IsActive()) {
		collider_.radius_ = GlobalVariables::GetInstance()->GetValue<Vector3>("Boss", "BarrierScale").x * worldTransform_.transform_.scale.x;
	}
	else {
		collider_.radius_ = GlobalVariables::GetInstance()->GetValue<Vector3>("Boss", "NormalScale").x * worldTransform_.transform_.scale.x;
	}
	// コライダーの更新
	collider_.Update(worldTransform_.GetWorldPosition());

	// 正面の方向ベクトル
	frontVector_ = Matrix4x4::TransformVector3(Vector3::Backward(), Matrix4x4::MakeRotateXYZMatrix(worldTransform_.transform_.rotate));

	AnimationUpdate();
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
		// 丸影
		roundShadow_->Draw(desc);
		// アニメーション
		animationManager_->Draw(desc.camera, lightDesc);
		// バリア
		systemManager_->barrierManager_.Draw(desc.camera, lightDesc);
	}
}

void Boss::ImGuiDraw()
{
	
	ImGui::Begin("Boss");
	if (ImGui::BeginTabBar("System"))
	{
		// 行動
		if (ImGui::BeginTabItem("ACTION")) {
			if (ImGui::Button("MissileState") || Input::GetInstance()->TriggerKey(DIK_7)) {
				stateManager_.ChangeRequest(std::make_unique<BossState::MissileAttackState>());
			}
			if (ImGui::Button("MissileBarrage") || Input::GetInstance()->TriggerKey(DIK_8)) {
				stateManager_.ChangeRequest(std::make_unique<BossState::MissileBarrageState>());
			}
			if (ImGui::Button("NormalAttack") || Input::GetInstance()->TriggerKey(DIK_9)) {
				stateManager_.ChangeRequest(std::make_unique<BossState::AttackState>());
			}
			if (ImGui::Button("MissileWave")) {
				stateManager_.ChangeRequest(std::make_unique<BossState::MissileWaveState>());
			}
			if (ImGui::Button("MissileContainer")) {
				stateManager_.ChangeRequest(std::make_unique<BossState::MissileContainerState>());
			}
			if (Input::GetInstance()->TriggerKey(DIK_Y)) {
				if (isAction_) {
					isAction_ = false;
				}
				else {
					isAction_ = true;
				}
			}
			ImGui::Checkbox("IsAction", &isAction_);
			ImGui::EndTabItem();
		}
		// ボス自体の状態
		if (ImGui::BeginTabItem("MAIN")) {
			ImGui::Checkbox("IsInvisible", &isInvisible_);
			ImGui::DragFloat3("Position", &worldTransform_.transform_.translate.x, 0.1f);
			ImGui::DragFloat3("Rotate", &worldTransform_.transform_.rotate.x, 0.01f);
			ImGui::DragFloat3("FrontVector", &frontVector_.x);
			ImGui::DragFloat3("Scale", &worldTransform_.transform_.scale.x, 0.01f);
			collider_.radius_ = worldTransform_.transform_.scale.x;
			Vector2 boss = { worldTransform_.GetWorldPosition().x,worldTransform_.GetWorldPosition().z };
			Vector2 player = { player_->worldTransform_.GetWorldPosition().x,player_->worldTransform_.GetWorldPosition().z };
			float distance = Vector2::Distance(boss, player);
			ImGui::DragFloat("PlayerDistance", &distance);
			ImGui::EndTabItem();
		}

		// アニメーション：モデル
		if (ImGui::BeginTabItem("ANIM")) {
			if (ImGui::Button("Open")) {
				animationManager_->AnimationExecute(BossSystemContext::AnimationManager::AnimType::kOpen, 60.0f);
			}
			if (ImGui::Button("Close")) {
				animationManager_->AnimationExecute(BossSystemContext::AnimationManager::AnimType::kClose, 60.0f);
			}

			// アニメーション
			animationManager_->ImGuiDraw();

			ImGui::EndTabItem();
		}

		// バリア関係
		if (ImGui::BeginTabItem("BARRIER")) {
			if (ImGui::Button("CreateBarrier")) {
				systemManager_->barrierManager_.Create(GlobalVariables::GetInstance()->GetValue<float>("Boss", "BarrierHP"));
			}
			if (ImGui::Button("BreakBarrier")) {
				systemManager_->barrierManager_.BarrierBreakExcept();
				systemManager_->particleManager_.BarrierBreakExcept();
			}
			systemManager_->barrierManager_.ImGuiDraw();
			ImGui::EndTabItem();
		}

		// UI
		if (ImGui::BeginTabItem("UI")) {
			systemManager_->uiManager_.ImGuiDraw();
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}

	// システムのタブ
	if (ImGui::BeginTabBar("BulletInfo"))
	{
		// 通常弾
		if (ImGui::BeginTabItem("Normal")) {

			ImGui::EndTabItem();
		}
		// 追尾弾
		if (ImGui::BeginTabItem("Tracking")) {

			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}

	systemManager_->ImGuiDraw();

	ImGui::End();

}

void Boss::OnCollision(ColliderObject target)
{
	// 弾との衝突
	if (std::holds_alternative<IBullet*>(target)) {
		IBullet** bullet = std::get_if<IBullet*>(&target);
		//Vector2 xzBullet = { (*bullet)->transform_.translate.x,(*bullet)->transform_.translate.z };	// 弾のXZ平面上座標
		//Vector2 xzBoss = { worldTransform_.GetWorldPosition().x ,worldTransform_.GetWorldPosition().z };	// ボスのXZ平面上座標
		//float distance = Vector2::Distance(xzBoss, xzBullet);
		float damageRatio = (*bullet)->DamageRatio();
		// バリアとの衝突処理
		if (systemManager_->barrierManager_.IsActive()) {
			float magnification = 2.0f;	// 倍率
			// ダメージの値
			damageRatio *= magnification;
			systemManager_->barrierManager_.DamageProcess(damageRatio);
			if (systemManager_->barrierManager_.IsShattered()) {
				systemManager_->barrierManager_.BarrierBreakExcept();
				systemManager_->particleManager_.BarrierBreakExcept();
			}

		}
		// 本体との衝突処理
		else {
			// 弱点むき出し状態のダメージ倍率アップ
			if (animationManager_->IsOpen()) {
				damageRatio *= 1.75f;
				// 点滅の処理
				animationManager_->AnimationDamageExecute();
			}
			systemManager_->healthManager_.TakeDamage(damageRatio);

			//// 距離に応じて
			//if (distance >= 150.0f) {
			//	systemManager_->healthManager_.TakeDamage(damageRatio * 0.25f);
			//}
			//else if (distance >= 75.0f) {
			//	systemManager_->healthManager_.TakeDamage(damageRatio * 0.5f);
			//}
			//else {
			//	systemManager_->healthManager_.TakeDamage(damageRatio);
			//}

			// オンヒットエフェクト
			systemManager_->particleManager_.OnBulletHit();

			// 死亡処理
			if (systemManager_->healthManager_.IsDead()) {
				isDead_ = true;
			}
		}
	}
}

void Boss::AnimationUpdate()
{
	// アニメ
	animationManager_->Update();

	// 死んでる場合パーティクル更新
	if (isDead_) {
		systemManager_->particleManager_.Update();
	}

}

void Boss::UIDraw()
{
	systemManager_->uiManager_.Draw();
}

void Boss::Finalize()
{
	// GPUParticleの登録を解除する処理
	gpuParticle_->DeleteEmitter("BossDamage");
	gpuParticle_->DeleteEmitter("BossBarrierBreak");
}

void Boss::SetCollier(CollisionManager* collisionManager)
{
	collisionManager->ListRegist(&collider_);
}

void Boss::InitializeGlobalValue()
{
	GlobalVariables* instance = GlobalVariables::GetInstance();
	std::string groupName = "Boss";
	//---ボスの共通---//
	instance->CreateGroup(groupName);
	// 座標関係
	instance->AddValue(groupName, "ResPosition", Vector3(0, 8.5f, 50.0f));
	instance->AddValue(groupName, "MinYPoint", float(12.0f));	
	instance->AddValue(groupName, "MaxYPoint", float(18.0f));
	instance->AddValue(groupName, "ScaleRate", float(1.0f));
	
	instance->AddValue(groupName, "NormalScale", worldTransform_.transform_.scale);
	// バリア
	instance->AddValue(groupName, "BarrierScale", worldTransform_.transform_.scale * 2.0f);
	instance->AddValue(groupName, "BarrierHP", 4.0f);
	instance->AddValue(groupName, "BarrierVanishFrame", 45.0f);
	instance->AddValue(groupName, "BarrierReappearFrame", 30.0f);
	instance->AddValue(groupName, "BarrierDissolveColor", Vector3(1.0f, 1.0f, 1.0f));

	//---ボスのエフェクトアニメーション---//
	groupName = "BossEffect";
	instance->AddValue(groupName, "DamageDistance", float(-5.0f));
	
	groupName = "BossAction";
	// 波状
	instance->AddValue(groupName, "WaveAttackDuration", float(30.0f));	// 
	instance->AddValue(groupName, "WaveAttackInitSpeed", float(100.0f));	//
	// コンテナ
	instance->AddValue(groupName, "ContainerInitSpeed", float(10.0f));

	groupName = "BossWaveAttack";
	instance->AddValue(groupName, "StFrame", float(120.0f));

	//---敵の弾のトレイル---//
	groupName = "BossBulletTrail";
	instance->CreateGroup(groupName);
	instance->AddValue(groupName, "Color", Vector3(1.0f, 0.6f, 0.6f));
	instance->AddValue(groupName, "MaxAlpha", float(1.0f));
	instance->AddValue(groupName, "MinAlpha", float(0.0f));

	//---通常弾---//
	groupName = "BossNormalBullet";
	instance->CreateGroup(groupName);
	instance->AddValue(groupName, "Scale", Vector3(1.0f, 1.0f, 1.0f));

	groupName = "BossAnimation";
	instance->CreateGroup(groupName);

	instance->AddValue(groupName, "OpenFrame", float(55.0f));
	instance->AddValue(groupName, "CloseFrame", float(30.0f));


	//---追尾弾---//
	groupName = "BossTrackingBullet";
	instance->CreateGroup(groupName);
	instance->AddValue(groupName, "TrackFrame", 0.0f);
	instance->AddValue(groupName, "Damping", 0.0f);
	instance->AddValue(groupName, "BaseSpeed", 0.0f);
	instance->AddValue(groupName, "InitSpeed", 0.0f);
	instance->AddValue(groupName, "LerpRadius", 0.0f);
	instance->AddValue(groupName, "Scale", Vector3(1.0f, 1.0f, 1.0f));
	instance->AddValue(groupName, "TrailSaveFrame", int32_t(50));
	instance->AddValue(groupName, "TrailMaxWidth", float(1.0f));
	instance->AddValue(groupName, "TrailMinWidth", float(0.25f));
	instance->AddValue(groupName, "StraightFrame", float(60.0f));
	instance->AddValue(groupName, "TrackingDot", float(-0.85f));	// 追従判定の値
	instance->AddValue(groupName, "TrackingDotLoose", float(-0.85f));	// 追従判定の値（緩め）

	// 通常


	// 優等生
	groupName = "TrackSuperior";
	instance->CreateGroup(groupName);
	instance->AddValue(groupName, "PredictionTime", float(6.0f));	// 予測フレーム

	// 劣等性
	groupName = "TrackInferior";
	instance->CreateGroup(groupName);
	instance->AddValue(groupName, "MinOffset", float(15.0f));	// ずらしの最小
	instance->AddValue(groupName, "MaxOffset", float(25.0f));	// ずらしの最大

	groupName = "TrackingData";
	instance->CreateGroup(groupName);
	instance->AddValue(groupName, "TrackingAccuracy", float(1.0f));

}

Vector3 Boss::HitEffectPosition()
{
	Vector3 direct = Vector3::Normalize(worldTransform_.GetWorldPosition() - camera_->transform_.translate);
	direct.y = 0.0f;
	return Vector3(direct * GlobalVariables::GetInstance()->GetValue<float>("BossEffect","DamageDistance"));
}