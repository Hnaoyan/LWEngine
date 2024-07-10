#include "Player.h"
#include "imgui.h"
#include "../GameObjectLists.h"
#include "../../GameSystem/GameSystem.h"

void Player::Initialize(Model* model)
{
	// 基底クラスの初期化
	IGameObject::Initialize(model);

	collider_.Initialize(worldTransform_.transform_.scale, this);
	collider_.SetAttribute(kCollisionAttributePlayer);
	// システム関係の初期化
	SystemInitialize();

}

void Player::Update()
{
	// 前フレの位置
	prevPosition_ = worldTransform_.GetWorldPosition();
	// システム関係の更新
	SystemUpdate();

	// 基底クラスの更新
	IGameObject::Update();
	// コライダー更新
	collider_.Update(worldTransform_.GetWorldPosition());
	// 足場コライダー
	footCollider_.Update();
}

void Player::Draw(ModelDrawDesc desc)
{
	ModelDrawDesc drawDesc{};
	// クラスの値設定
	drawDesc.worldTransform = &worldTransform_;
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
	ImGui::DragFloat3("Velocity", &velocity_.x);

	// システムのタブ
	if (ImGui::BeginTabBar("System"))
	{
		if (ImGui::BeginTabItem("Aim")) {
			aimManager_.ImGuiDraw();
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("FootCollider")) {
			footCollider_.ImGuiDraw();
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("S3")) {

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
	target;
}

void Player::UISpriteDraw()
{
	aimManager_.Draw();
}

void Player::SystemInitialize()
{
	// 操作システム
	systemManager_.Initialize(this);
	// エイムシステム
	aimManager_.Initialize(this);
	// 足場コライダー
	footCollider_.Initialize(this);
}

void Player::SystemUpdate()
{
	systemManager_.Update();

	aimManager_.Update(camera_);
}

void Player::CollisionCorrect(ICollider::CollisionType3D type, const Vector3& min, const Vector3& max)
{
	// 修正座標（最後にこれを渡す	
	Vector3 correctPosition = worldTransform_.GetWorldPosition();
	Vector3 prevToNow = prevPosition_ - worldTransform_.GetWorldPosition();

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
		if (prevToNow.y == 0.0f) {
			correctPosition.x = max.x + worldTransform_.transform_.scale.x;
		}
		break;
	case ICollider::kLeftTopSide:
		if (prevToNow.y == 0.0f) {
			correctPosition.x = max.x + worldTransform_.transform_.scale.x;
		}
		break;
	case ICollider::kLeftFrontSide:
		if (prevToNow.y == 0.0f) {
			correctPosition.x = max.x + worldTransform_.transform_.scale.x;
		}
		break;
	case ICollider::kLeftBackSide:
		if (prevToNow.y == 0.0f) {
			correctPosition.x = max.x + worldTransform_.transform_.scale.x;
		}
		break;
	case ICollider::kRightBottomSide:
		if (prevToNow.y == 0.0f) {
			correctPosition.x = min.x - worldTransform_.transform_.scale.x;
		}
		break;
	case ICollider::kRightTopSide:
		if (prevToNow.y == 0.0f) {
			correctPosition.x = min.x - worldTransform_.transform_.scale.x;
		}
		break;
	case ICollider::kRightFrontSide:
		if (prevToNow.y == 0.0f) {
			correctPosition.x = min.x - worldTransform_.transform_.scale.x;
		}
		break;
	case ICollider::kRightBackSide:
		if (prevToNow.y == 0.0f) {
			correctPosition.x = min.x - worldTransform_.transform_.scale.x;
		}
		break;
	case ICollider::kFrontBottomSide:
		if (prevToNow.y == 0.0f) {
			correctPosition.z = max.z + worldTransform_.transform_.scale.z;
		}
		break;
	case ICollider::kFrontTopSide:
		if (prevToNow.y == 0.0f) {
			correctPosition.z = max.z + worldTransform_.transform_.scale.z;
		}
		break;
	case ICollider::kBackBottomSide:
		if (prevToNow.y == 0.0f) {
			correctPosition.z = min.z - worldTransform_.transform_.scale.z;
		}
		break;
	case ICollider::kBackTopSide:
		if (prevToNow.y == 0.0f) {
			correctPosition.z = min.z - worldTransform_.transform_.scale.z;
		}
		break;
	//---面---//
	case ICollider::kFrontFace:
		correctPosition.z = max.z + worldTransform_.transform_.scale.z;
		break;
	case ICollider::kBackFace:
		correctPosition.z = min.z - worldTransform_.transform_.scale.z;
		break;
	case ICollider::kLeftFace:
		correctPosition.x = max.x + worldTransform_.transform_.scale.x;
		break;
	case ICollider::kRightFace:
		correctPosition.x = min.x - worldTransform_.transform_.scale.x;
		break;
	case ICollider::kTopFace:
		correctPosition.y = min.y - worldTransform_.transform_.scale.y;
		break;
	case ICollider::kBottomFace:
		correctPosition.y = max.y + worldTransform_.transform_.scale.y + 0.02f;
		break;
	// 例外
	case ICollider::kMultiplePoints:
		break;
	case ICollider::kNone:
		break;
	}

	worldTransform_.transform_.translate = correctPosition;

}
