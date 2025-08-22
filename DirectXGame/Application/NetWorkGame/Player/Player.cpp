#include "Player.h"
#include "Engine/Collision/2D/Collider2DLists.h"
#include "Application/Collision/ColliderFilter.h"
#include "Engine/Physics/PenetrationResolver.h"

#include "Application/NetWorkGame/NetworkGameObjectLists.h"

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

	// 操作クラス
	controller_ = std::make_unique<PlayerController>();
	controller_->Initialize(this);
}

void Player::Update()
{

	controller_->Update();

	// 基底更新
	IGameObject2D::Update();
}

void Player::ImGuiDraw()
{

}

void Player::OnCollision([[maybe_unused]] ColliderObject target)
{
	if (std::holds_alternative<Obstacle*>(target)) {
		//Obstacle** obj = std::get_if<Obstacle*>(&target);
		//Rectangle2D* rectP = std::get<Rectangle2D*>(*collider_);
		//Rectangle2D* rectT = std::get<Rectangle2D*>((*obj).)
		//PenetrationResolver::Extrusion2DCalculation(*rectP, )
	}
	
}