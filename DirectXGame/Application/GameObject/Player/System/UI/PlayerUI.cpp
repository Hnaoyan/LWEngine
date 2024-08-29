#include "PlayerUI.h"
#include "../../Player.h"
#include "Engine/2D/SpriteManager.h"

void PlayerContext::UIManager::Initialize(Player* player)
{
	player_ = player;

	reticle_ = SpriteManager::GetSprite("CrossHair");

}

void PlayerContext::UIManager::Draw()
{
	reticle_->SetPosition(player_->GetOperation()->GetAimManager()->GetReticlePosition());
	reticle_->Draw();
}
