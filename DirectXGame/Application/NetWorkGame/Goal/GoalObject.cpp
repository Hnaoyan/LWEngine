#include "GoalObject.h"
#include "Application/Collision/ColliderFilter.h"

#include <imgui.h>

void GoalObject::Initialize(Model* model)
{
	IGameObject2D::Initialize(model);

	Rectangle2D* rect = new Rectangle2D();
	rect->radius_ = 1.0f;
	rect->Initialize(this);
	rect->SetAttribute(kCollisionAttributeGoal);
	rect->SetMask(kCollisionAttributePlayer);
	Collider2DShape* shape = new Collider2DShape();
	*shape = *rect;
	collider_.reset(shape);
	worldTransform_.transform_.rotate.y = 1.57f;
}

void GoalObject::Update()
{
	// 基底更新
	IGameObject2D::Update();
}

void GoalObject::ImGuiDraw()
{
	ImGui::Begin("Goal");

	ImGui::DragFloat3("Position", &worldTransform_.transform_.translate.x, 0.01f);

	ImGui::End();
}

void GoalObject::OnCollision([[maybe_unused]] ColliderObject target)
{

}
