#include "BossUI.h"
#include "Engine/2D/SpriteManager.h"
#include "Engine/2D/TextureManager.h"

void BossSystemContext::UIManager::Initialize(Boss* boss)
{
	boss_ = boss;

	//uint32_t texture = TextureManager::GetInstance()->Load("Resources/default/white2x2.png");
	hpUI_.backGround.Initialize("GageBack");

}

void BossSystemContext::UIManager::Draw()
{
	hpUI_.backGround.Update();
	hpUI_.backGround.sprite->Draw();
}
