#include "Player.h"
#include "imgui.h"
#include "../GameObjectLists.h"
#include "Application/GameSystem/GameSystem.h"
#include "Module/State/PlayerStateList.h"

#include "Engine/Collision/CollisionManager.h"
#include "Engine/PostEffect/PostEffectRender.h"
#include "Engine/3D/ModelUtility/ModelRenderer.h"
#include "Engine/3D/ModelUtility/ModelManager.h"
#include "Engine/2D/TextureManager.h"
#include "Engine/LwLib/Ease/Ease.h"

void Player::PreInitialize(ICamera* camera, GPUParticleManager* gpuParticle)
{
	assert(camera);
	assert(gpuParticle);
	// カメラ
	camera_ = camera;
	// システム集積クラスの初期化
	facadeSystem_ = std::make_unique<PlayerFacade>();
	facadeSystem_->GetParticleManager()->SetGPUParticleSystem(gpuParticle);
}

void Player::Initialize(Model* model)
{
	// 基底クラスの初期化
	IGameObject::Initialize(model);
	// マテリアル
	material_ = std::make_unique<Material>();
	material_->CreateMaterial();
	material_->discardThreshold_ = 0.075f;
	// 影
	roundShadow_->SetDefaultScale(Vector2(3.0f, 3.0f));
	// トランスフォーム
	worldTransform_.transform_.rotate = {};
	worldTransform_.transform_.translate = GlobalVariables::GetInstance()->GetValue<Vector3>("Player", "InitPosition");
	worldTransform_.UpdateMatrix();
	// コライダー
	collider_.Initialize(worldTransform_.transform_.scale, this);
	collider_.SetAttribute(kCollisionAttributePlayer);
	// システム関係の初期化
	oparationManager_.Initialize(this);
	// ファサード初期化
	facadeSystem_->Initialize(this);

	// 足場コライダー
	footCollider_.Initialize(this);

	// ステート系の初期化
	stateManager_ = std::make_unique<PlayerStateManager>();
	stateManager_->Initialize(this);

	// 武器系
	weaponManager_ = std::make_unique<WeaponManager>(this);
	weaponManager_->Initialize();

}

void Player::Update()
{
	// 前フレの位置
	prevPosition_ = worldTransform_.GetWorldPosition();
	// システム関係の更新
	facadeSystem_->Update();
	// システム関係
	oparationManager_.Update();
	// それぞれの軸のマネージャ
	stateManager_->Update();
	// 武器
	weaponManager_->Update();
	// 基底クラスの更新
	IGameObject::Update();
	// 角度に合わせて透明化する処理
	RotateCleanness();

	// コライダー更新
	collider_.Update(worldTransform_.GetWorldPosition());
	// 足場コライダー
	footCollider_.Update();
}

void Player::Draw(ModelDrawDesc desc)
{
	// マテリアル更新
	model_->GetMaterial()->Update();
	material_->Update();
	// デスクの設定
	DrawDesc::LightDesc lightDesc{};
	DrawDesc::ModelDesc modelDesc{};
	lightDesc.directionalLight = desc.directionalLight;
	lightDesc.pointLight = desc.pointLight;
	lightDesc.spotLight = desc.spotLight;
	modelDesc.SetDesc(model_);
	modelDesc.worldTransform = &facadeSystem_->GetAnimation()->bodyTransform_;
	modelDesc.material = material_.get();


	// プレイヤーの描画
	if (!isInvisible_) {
		// 本体の描画
		ModelRenderer::NormalDraw(desc.camera, modelDesc, lightDesc);
		//facadeSystem_->GetDudgeManager()->Draw(desc);

		facadeSystem_->GetUI()->Draw(desc);
		roundShadow_->Draw(desc);
	}

	// パーティクル
	facadeSystem_->GetParticleManager()->Draw(desc.camera);
}

void Player::ImGuiDraw()
{
	std::string name = "Player";
	ImGui::Begin(name.c_str());
	ImGui::DragFloat("Discard", &material_->discardThreshold_, 0.01f);
	//---基本情報---//
	ImGui::SeparatorText("StatusInfo");
	NowState();	// ステート
	ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(250, 200), ImGuiWindowFlags_NoTitleBar);
	ImGui::Checkbox("IsInvisible", &isInvisible_);
	ImGui::DragFloat3("Position", &worldTransform_.transform_.translate.x, 0.01f);
	ImGui::DragFloat3("Rotate", &worldTransform_.transform_.rotate.x, 0.01f);
	ImGui::DragFloat3("Scale", &worldTransform_.transform_.scale.x, 0.01f);
	ImGui::DragFloat3("Velocity", &velocity_.x);
	ImGui::Text("IsGround:%d", this->isGround_);
	bool isInv = facadeSystem_->GetDudgeManager()->IsInvisibleActive();
	ImGui::Text("IsInvisible:%d", isInv);

	// 
	ImGui::DragFloat("InvisibleFrame", &invisibleFrame_, 0.01f);
	ImGui::DragFloat("EnergyRecover", &energyRecover_, 0.01f);
	ImGui::DragFloat("TrackCancel", &trackCancelDistance, 0.01f);

	ImGui::EndChild();

	// システムのタブ
	if (ImGui::BeginTabBar("System"))
	{
		if (ImGui::BeginTabItem("Dodge")) {
			facadeSystem_->GetDudgeManager()->ImGuiDraw();
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Health")) {
			facadeSystem_->GetHealth()->ImGuiDraw();
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Aim")) {
			weaponManager_->GetAimManager()->ImGuiDraw();
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("LockOn")) {
			weaponManager_->GetLockOn()->ImGuiDraw();
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("FootCollider")) {
			footCollider_.ImGuiDraw();
			roundShadow_->ImGuiDraw();
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Energy")) {
			facadeSystem_->GetEnergy()->ImGuiDraw();
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("UI")) {
			facadeSystem_->GetUI()->ImGuiDraw();
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Material")) {
			ImGui::DragFloat4("ModelColor", &material_->color_.x, 0.01f);
			ImGui::DragFloat("MaterialShininess", &material_->shininess_, 0.01f);
			ImGui::DragFloat("Threshold", &material_->threshold_, 0.01f, 0.0f, 1.0f);
			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();
	}

	ImGui::End();
}

void Player::OnCollision(ColliderObject target)
{
	if (std::holds_alternative<Terrain*>(target)) {
		Terrain** terrain = std::get_if<Terrain*>(&target);
		Vector3 correctPos = {};

		Vector3 min{}, max{};

		max = (*terrain)->GetCollider()->max_;
		min = (*terrain)->GetCollider()->min_;

		ICollider::BoxVertices box = ICollider::CreateBoxVertices(collider_.min_, collider_.max_);

		ICollider::CollisionType3D type = ICollider::GetCollisionType3D(box, min, max);

		// 座標修正関数
		CollisionCorrect(type, min, max);
	}
	else if (std::holds_alternative<IBullet*>(target)) {
		// ジャスト回避処理
		if (facadeSystem_->GetDudgeManager()->IsActive() && !facadeSystem_->GetDudgeManager()->IsInvisibleActive()) 
		{
			facadeSystem_->GetDudgeManager()->InvisibleExcept(invisibleFrame_);
			facadeSystem_->GetEnergy()->RecoverGage(energyRecover_);
			float returnFrame = 45.0f;
			PostEffectManager::sSlowEffect.Initialize(invisibleFrame_ + returnFrame);
		}
		// ジャスト回避後の猶予
		else if (facadeSystem_->GetDudgeManager()->IsInvisibleAfter()) {

		}
		// ダメージを受ける処理
		else {
			facadeSystem_->GetHealth()->TakeDamage();
		}
	}
}

void Player::UIDraw()
{
	facadeSystem_->GetUI()->Draw();
}

void Player::SetCollier(CollisionManager* collisionManager)
{
	collisionManager->ListRegist(&collider_);
	collisionManager->ListRegist(footCollider_.GetCollider());
	if (facadeSystem_->GetDudgeManager()->IsActive()) {
		collisionManager->ListRegist(facadeSystem_->GetDudgeManager()->GetCollider());
	}
}

void Player::InitializeGlobalValue()
{
	GlobalVariables* instance = GlobalVariables::GetInstance();
	//---プレイヤー基本情報---//
	std::string groupName = "Player";
	instance->CreateGroup(groupName);
	// ダッシュ関係
	instance->AddValue(groupName, "DashPower", float(0.0f));
	instance->AddValue(groupName, "QuickBoostEndTime", float(40.0f));
	instance->AddValue(groupName, "BoostDashPower", float(0.0f));
	instance->AddValue(groupName, "BoostEndTime", float(30.0f));
	instance->AddValue(groupName, "DashCooltime", float(30.0f));
	instance->AddValue(groupName, "DashExceptTime", float(12.0f));

	// 基本部分
	instance->AddValue(groupName, "InitPosition", Vector3(0.0f, -35.0f, 0.0f));	// 初期座標
	instance->AddValue(groupName, "VelocityDecay", float(0.2f));	// 減速割合
	instance->AddValue(groupName, "MoveSpeed", float(10.0f));	// 移動速度
	instance->AddValue(groupName, "ShotDuration", float(30.0f));	// 撃つ間隔
	instance->AddValue(groupName, "LockDuration", float(25.0f));	// ロックオン切り替え間隔
	instance->AddValue(groupName, "HitPoint", int32_t(20));	// HP
	instance->AddValue(groupName, "AimOffset", Vector3(0.0f, 0.0f, 50.0f));	// オフセット距離
	instance->AddValue(groupName, "FirstJumpPower", float(65.0f));	// 初期ジャンプ力
	instance->AddValue(groupName, "SecondJumpPower", float(65.0f));	// 2段目ジャンプ力
	instance->AddValue(groupName, "FallGravity", float(-3.0f));	// 落下の重力

	//---（仮）弾---//
	groupName = "Bullet";
	// 弾本体
	instance->AddValue(groupName, "InferiorColor", Vector4(0.0f, 0.0f, 0.0f));
	instance->AddValue(groupName, "SuperirorColor", Vector4(0.0f, 0.0f, 0.0f));
	instance->AddValue(groupName, "GeniusColor", Vector4(0.0f, 0.0f, 0.0f));
	instance->AddValue(groupName, "ContainerColor", Vector4(0.0f, 0.0f, 0.0f));
	instance->AddValue(groupName, "NormalColor", Vector4(0.0f, 0.0f, 0.0f, 1.0f));
	// 軌跡
	instance->AddValue(groupName, "InferiorTrailColor", Vector3(0.0f, 0.0f, 0.0f));
	instance->AddValue(groupName, "SuperirorTrailColor", Vector3(0.0f, 0.0f, 0.0f));
	instance->AddValue(groupName, "GeniusTrailColor", Vector3(0.0f, 0.0f, 0.0f));
	instance->AddValue(groupName, "ContainerTrailColor", Vector3(0.0f, 0.0f, 0.0f));
	instance->AddValue(groupName, "NormalTrailColor", Vector3(0.2f, 0.2f, 0.2f));

	//---プレイヤーの追従弾---//
	groupName = "PlayerTrackingBullet";
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
}

void Player::NowState()
{
	PlayerState horizon = stateManager_->GetHorizontal()->GetVariant();
	PlayerState vertical = stateManager_->GetVertical()->GetVariant();
	std::string horizonName;
	std::string verticalName;
	if (std::holds_alternative<AssendingState*>(vertical)) {
		verticalName = "Assending";
	}
	else if (std::holds_alternative<FallingState*>(vertical)) {
		verticalName = "Falling";
	}
	else if (std::holds_alternative<IdleVertical*>(vertical)) {
		verticalName = "Idle";
	}
	else if (std::holds_alternative<JumpingState*>(vertical)) {
		verticalName = "Jumping";
	}

	//if (std::holds_alternative<BoostState*>(horizon)) {
	//	horizonName = "Boost";
	//}
	//else if (std::holds_alternative<IdleHorizontal*>(horizon)) {
	//	horizonName = "Idle";
	//}
	//else if (std::holds_alternative<MovingState*>(horizon)) {
	//	horizonName = "Move";
	//}
	//else if (std::holds_alternative<QuickBoostState>(horizon)) {
	//	horizonName = "Quick";
	//}
	verticalName = "VerticalState:" + verticalName;
	horizonName = "HorizontalState:" + horizonName;

	ImGui::Text(verticalName.c_str());
	ImGui::Text(horizonName.c_str());
}

void Player::RotateCleanness()
{
	// 画角に応じて透明化するように
	if (camera_) {
		float min = -0.1f;
		float max = -0.3f;
		float rotX = camera_->transform_.rotate.x;
		float v = (rotX - min) / (max - min);
		v = std::clamp(v, 0.0f, 1.0f);

		material_->color_.w = Ease::Easing(1.0f, 0.2f, v);
	}
}

void Player::CollisionCorrect(ICollider::CollisionType3D type, const Vector3& min, const Vector3& max)
{
	// 修正座標	
	Vector3 correctPosition = worldTransform_.GetWorldPosition();
	Vector3 prevToNow = prevPosition_ - worldTransform_.GetWorldPosition();

	bool isXCorrect = false;
	bool isYCorrect = false;
	bool isZCorrect = false;

	float correctValue = 0.025f;

	Vector3 playerScale = worldTransform_.transform_.scale;
	playerScale.x += (correctValue + correctValue);
	playerScale.y += correctValue;
	playerScale.z += correctValue;

	switch (type)
	{
	//---点---//
	case ICollider::kLeftBottomFront:

		break;
	case ICollider::kLeftBottomBack:

		break;
	case ICollider::kLeftTopFront:

		break;
	case ICollider::kLeftTopBack:

		break;
	case ICollider::kRightBottomFront:

		break;
	case ICollider::kRightBottomBack:

		break;
	case ICollider::kRightTopFront:

		break;
	case ICollider::kRightTopBack:

		break;
	//---辺---//
	case ICollider::kLeftBottomSide:
		//if (prevToNow.y == 0.0f) {
			correctPosition.x = max.x + (playerScale.x);
			isXCorrect = true;
		//}
		break;
	case ICollider::kLeftTopSide:
		//if (prevToNow.y == 0.0f) {
			correctPosition.x = max.x + (playerScale.x);
			isXCorrect = true;
		//}
		break;
	case ICollider::kLeftFrontSide:
		//if (prevToNow.y == 0.0f) {
			correctPosition.x = max.x + (playerScale.x);
			isXCorrect = true;
		//}
		break;
	case ICollider::kLeftBackSide:
		//if (prevToNow.y == 0.0f) {
			correctPosition.x = max.x + (playerScale.x);
			isXCorrect = true;
		//}
		break;
	case ICollider::kRightBottomSide:
		//if (prevToNow.y == 0.0f) {
			correctPosition.x = min.x - (playerScale.x);
			isXCorrect = true;
		//}
		break;
	case ICollider::kRightTopSide:
		//if (prevToNow.y == 0.0f) {
			correctPosition.x = min.x - (playerScale.x);
			isXCorrect = true;
		//}
		break;
	case ICollider::kRightFrontSide:
		//if (prevToNow.y == 0.0f) {
			correctPosition.x = min.x - (playerScale.x);
			isXCorrect = true;
		//}
		break;
	case ICollider::kRightBackSide:
		//if (prevToNow.y == 0.0f) {
			correctPosition.x = min.x - (playerScale.x);
			isXCorrect = true;
		//}
		break;
	case ICollider::kFrontBottomSide:
		//if (prevToNow.y == 0.0f) {
			correctPosition.z = max.z + (playerScale.z);
			isZCorrect = true;
		//}
		break;
	case ICollider::kFrontTopSide:
		//if (prevToNow.y == 0.0f) {
			correctPosition.z = max.z + (playerScale.z);
			isZCorrect = true;
		//}
		break;
	case ICollider::kBackBottomSide:
		//if (prevToNow.y == 0.0f) {
			correctPosition.z = min.z - (playerScale.z);
			isZCorrect = true;
		//}
		break;
	case ICollider::kBackTopSide:
		//if (prevToNow.y == 0.0f) {
			correctPosition.z = min.z - (playerScale.z);
			isZCorrect = true;
		//}
		break;
	//---面---//
	case ICollider::kFrontFace:
		correctPosition.z = max.z + (playerScale.z);
		isZCorrect = true;
		break;
	case ICollider::kBackFace:
		correctPosition.z = min.z - (playerScale.z);
		isZCorrect = true;
		break;
	case ICollider::kLeftFace:
		correctPosition.x = max.x + (playerScale.x);
		isXCorrect = true;
		break;
	case ICollider::kRightFace:
		correctPosition.x = min.x - (playerScale.x);
		isXCorrect = true;
		break;
	case ICollider::kTopFace:
		correctPosition.y = min.y - (playerScale.y);
		isYCorrect = true;
		break;
	case ICollider::kBottomFace:
		correctPosition.y = max.y + (playerScale.y);
		isYCorrect = true;
		break;
	// 例外
	case ICollider::kMultiplePoints:
		if (worldTransform_.GetWorldPosition().y < max.y) {
			correctPosition.y = max.y + (playerScale.y);
			isYCorrect = true;
		}
		break;
	case ICollider::kNone:
		break;
	}
	if (isXCorrect) {
		velocity_.x = 0;
	}
	if (isYCorrect) {
		velocity_.y = 0;
	}
	if (isZCorrect) {
		velocity_.z = 0;
	}
	worldTransform_.transform_.translate = correctPosition;

}
