#include "Player.h"
#include "Engine/Collision/2D/Collider2DLists.h"
#include "Application/Collision/ColliderFilter.h"

void Player::Initialize(Model* model)
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

}

void Player::Update()
{

	// 基底更新
	IGameObject2D::Update();
}

void Player::ImGuiDraw()
{

}

void Player::OnCollision([[maybe_unused]] ColliderObject target)
{

}