#include "PlayerFootCollider.h"
#include "../../Player.h"
#include "Application/Collision/ColliderFilter.h"
#include <imgui.h>

void PlayerFootCollider::Initialize(Player* player)
{
	assert(player);
	player_ = player;

	worldTransform_.Initialize();
	worldTransform_.parent_ = player_->GetWorldTransform();
	const float yOffset = GlobalVariables::GetInstance()->GetValue<float>("PlayerBasic", "FootOffset");
	worldTransform_.transform_.translate = { 0,yOffset,0 };
	scale_ = { 1.0f,1.0f,1.0f };
	collider_.Initialize(scale_, this);
	collider_.SetAttribute(kCollisionAttributeFootCollider);
}

void PlayerFootCollider::Update()
{
	// コライダーの位置更新
	worldTransform_.UpdateMatrix();
	collider_.Update(worldTransform_.GetWorldPosition());
	// フラグ初期化
	player_->SetIsGround(false);
}

void PlayerFootCollider::OnCollision(ColliderObject target)
{
	if (std::holds_alternative<Terrain*>(target)) {
		player_->SetIsGround(true);
	}
}

void PlayerFootCollider::ImGuiDraw()
{
	ImGui::DragFloat3("FootTraslate", &worldTransform_.transform_.translate.x, 0.01f);
	ImGui::DragFloat3("FootScale", &scale_.x, 0.01f);
	Vector3 world = worldTransform_.GetWorldPosition();
	ImGui::DragFloat3("FootWorld", &world.x);
}
