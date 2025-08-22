#include "Player.h"

void Player::Initialize(Model* model)
{
	IGameObject2D::Initialize(model);
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