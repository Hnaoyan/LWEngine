#include "GameUIManager.h"
#include "Engine/2D/SpriteManager.h"

void GameUI::UIManager::Initialize()
{

}

void GameUI::UIManager::Draw()
{

}

void GameUI::SpriteInfo::Initialize(std::string spriteTag)
{
	sprite = SpriteManager::GetSprite(spriteTag);
	color = sprite->GetColor();
}

void GameUI::SpriteInfo::Update()
{

	sprite->SetColor(color);

}
