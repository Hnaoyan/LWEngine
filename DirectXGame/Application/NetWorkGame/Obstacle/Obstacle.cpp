#include "Obstacle.h"
#include "Engine/Collision/2D/Collider2DLists.h"
#include "Application/Collision/ColliderFilter.h"
#include "Engine/Collision/CollisionManager.h"

void Obstacle::Initialize(Model* model)
{
	IGameObject2D::Initialize(model);

	Rectangle2D* rect = new Rectangle2D();
	rect->radius_ = 1.0f;
	rect->Initialize(this);
	rect->SetAttribute(kCollisionAttributeTerrain);
	rect->SetMask(kCollisionAttributePlayer);
	Collider2DShape* shape = new Collider2DShape();
	*shape = *rect;
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

Rectangle2D* Obstacle::GetCollider()
{
	return std::get_if<Rectangle2D>(collider_.get());
}

void Obstacle::SetCollision(CollisionManager* manager)
{
	manager->ListRegist(*collider_.get());
}
