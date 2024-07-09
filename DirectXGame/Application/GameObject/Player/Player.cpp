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

		if (type == ICollider::CollisionType3D::kMultiplePoints) {
			correctPos.y = (*terrain)->GetWorldPosition().y + (*terrain)->GetTransform().scale.y + worldTransform_.transform_.scale.y;
			worldTransform_.transform_.translate.y = correctPos.y;
		}

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
