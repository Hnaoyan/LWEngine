#include "UIStructs.h"
#include "Engine/2D/SpriteManager.h"
#include "Application/GameObject/GameObjectLists.h"

void GameUI::SpriteInfo::Initialize(std::string spriteTag)
{
	sprite = SpriteManager::GetSprite(spriteTag);
	color = sprite->GetColor();
}

void GameUI::SpriteInfo::Update()
{

	sprite->SetColor(color);

}

void GameUI::SpriteInfo::Draw()
{
	Update();

	sprite->Draw();
}

