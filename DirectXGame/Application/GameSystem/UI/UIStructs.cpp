#include "UIStructs.h"
#include "Engine/2D/SpriteManager.h"
#include "Application/GameObject/GameObjectLists.h"
#include <imgui.h>

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

void GameUI::UIData::ImGuiDraw()
{

	ImGui::Begin(this->tag.c_str());

	std::string name = tag + "Pos";

	ImGui::DragFloat2(name.c_str(), &position.x, 0.01f);

	ImGui::End();

}
