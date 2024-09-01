#include "Player.h"
#include "imgui.h"
#include "../GameObjectLists.h"
#include "Application/GameSystem/GameSystem.h"

#include "Engine/PostEffect/PostEffectRender.h"

void Player::Initialize(Model* model)
{
	// 基底クラスの初期化
	IGameObject::Initialize(model);

	worldTransform_.transform_.translate.z = -15.0f;
	worldTransform_.UpdateMatrix();

	collider_.Initialize(worldTransform_.transform_.scale, this);
	collider_.SetAttribute(kCollisionAttributePlayer);
	// システム関係の初期化
	systemManager_.Initialize(this);
	stateManager_.Initialize(this);
	healthManager_.Initialize(this, 10);
	particleManager_.Initialize(this);
	energyManager_.Initialize(this);
	uiManager_.Initialize(this);

	// 足場コライダー
	footCollider_.Initialize(this);

	modelTransform_.Initialize();
	modelTransform_.parent_ = &worldTransform_;
}

void Player::Update()
{
	// 前フレの位置
	prevPosition_ = worldTransform_.GetWorldPosition();
	// システム関係の更新
	systemManager_.Update();
	healthManager_.Update();
	energyManager_.Update();
	quickBoostCoolTime_.Update();

	if (currentStates_.first) {
		currentStates_.first->InputHandle();
		currentStates_.first->Update();
	}
	if (currentStates_.second) {
		currentStates_.second->InputHandle();
		currentStates_.second->Update();
	}

	// 基底クラスの更新
	IGameObject::Update();
	// 1フレームでのパラメータ加算値
	const float step = 2.0f * float(3.14f) / period;

	// パラメータを1ステップ分加算
	floatingParameter_ += step;
	// 2πを超えたら0に戻す
	floatingParameter_ = std::fmod(floatingParameter_, 2.0f * float(3.14f));
	// 浮遊を座標に反映
	modelTransform_.transform_.translate.y = std::sin(floatingParameter_) * floatingWidth;

	modelTransform_.UpdateMatrix();
	// コライダー更新
	collider_.Update(worldTransform_.GetWorldPosition());
	// 足場コライダー
	footCollider_.Update();
}

void Player::Draw(ModelDrawDesc desc)
{
	ModelDrawDesc drawDesc{};
	// クラスの値設定
	drawDesc.worldTransform = &modelTransform_;
	// 引数の値設定
	drawDesc.camera = desc.camera;
	drawDesc.directionalLight = desc.directionalLight;
	drawDesc.pointLight = desc.pointLight;
	drawDesc.spotLight = desc.spotLight;
	// プレイヤーの描画
	model_->Draw(drawDesc);
}

void Player::ImGuiDraw()
{
	std::string name = "Player";
	ImGui::Begin(name.c_str());
	ImGui::Text("IsGround:%d", this->isGround_);
	ImGui::DragFloat3("Position", &worldTransform_.transform_.translate.x, 0.01f);
	ImGui::DragFloat3("Rotate", &worldTransform_.transform_.rotate.x, 0.01f);
	ImGui::DragFloat3("Scale", &worldTransform_.transform_.scale.x, 0.01f);

	collider_.SetRadius(worldTransform_.transform_.scale * 0.75f);
	ImGui::DragFloat3("Velocity", &velocity_.x);

	// システムのタブ
	if (ImGui::BeginTabBar("System"))
	{
		if (ImGui::BeginTabItem("Aim")) {
			systemManager_.GetAimManager()->ImGuiDraw();
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("LockOn")) {
			systemManager_.GetLockOn()->ImGuiDraw();
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("FootCollider")) {
			footCollider_.ImGuiDraw();
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Energy")) {
			ImGui::DragFloat("Energy", &energyManager_.energy_.currentEnergy);
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("UI")) {
			uiManager_.ImGuiDraw();
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

		//if (type == ICollider::CollisionType3D::kBottomFace) {
		//	correctPos.y = (*terrain)->GetWorldPosition().y + (*terrain)->GetTransform().scale.y + worldTransform_.transform_.scale.y;
		//	worldTransform_.transform_.translate.y = correctPos.y;
		//}

	}

	else if (std::holds_alternative<BossSystemContext::NormalBullet*>(target)) {
		healthManager_.TakeDamage();
	}
	else if (std::holds_alternative<BossSystemContext::TrackingBullet*>(target)) {
		healthManager_.TakeDamage();
	}

}

void Player::UISpriteDraw()
{
	uiManager_.Draw();
}

void Player::CollisionCorrect(ICollider::CollisionType3D type, const Vector3& min, const Vector3& max)
{
	// 修正座標（最後にこれを渡す	
	Vector3 correctPosition = worldTransform_.GetWorldPosition();
	Vector3 prevToNow = prevPosition_ - worldTransform_.GetWorldPosition();

	bool isXCorrect = false;
	bool isYCorrect = false;
	bool isZCorrect = false;

	float correctValue = 0.025f;

	Vector3 playerScale = worldTransform_.transform_.scale;
	playerScale.x += correctValue + 0.025f;
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
