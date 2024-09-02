#include "BoostState.h"
#include "Application/GameSystem/GameSystem.h"
#include "Application/GameObject/Player/Player.h"
#include "Application/GameObject/Bullet/BulletManager.h"

void BoostState::Initialize()
{
	nowState_ = this;

}

void BoostState::Update()
{

}

void BoostState::Exit()
{
	player_->GetAnimationManager()->Reset();
}

void BoostState::InputHandle()
{

}
