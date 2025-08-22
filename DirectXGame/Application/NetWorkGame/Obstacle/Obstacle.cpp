#include "Obstacle.h"
#include "Engine/Collision/2D/Collider2DLists.h"
#include "Application/Collision/ColliderFilter.h"

void Obstacle::Initialize(Model* model)
{
	IGameObject2D::Initialize(model);

	Rectangle2D* rect = new Rectangle2D();
	rect->radius_ = 1.0f;
	rect->Initialize(this);
	rect->SetAttribute(kCollisionAttributePlayer);
	rect->SetMask(kCollisionAttributeTerrain);
	Collider2DShape* shape = new Collider2DShape();
	*shape = rect;
	collider_.reset(shape);

	worldTransform_.transform_.translate = { 5.0f,0.0f,0.0f };
}

void Obstacle::Update()
{
	IGameObject2D::Update();
}

void Obstacle::ImGuiDraw()
{

}

void Obstacle::OnCollision([[maybe_unused]] ColliderObject target)
{
}
