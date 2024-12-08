#include "HoverState.h"
#include "../../../Player.h"
#include "Application/GameSystem/GameSystem.h"

void HoverState::Initialize()
{
	nowState_ = this;
}

void HoverState::Update()
{

}

void HoverState::Exit()
{

}

void HoverState::InputHandle()
{
	//if(GameSystem::sKeyConfigManager.GetPlayerKey())
}
