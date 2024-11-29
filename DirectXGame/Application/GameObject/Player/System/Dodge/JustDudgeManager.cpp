#include "JustDudgeManager.h"
#include "../../Player.h"
#include <imgui.h>

PlayerContext::JustDodgeManager::JustDodgeManager(Player* player)
{
	// 設定
	player_ = player;
	// 残像用コライダー
	collider_.Initialize(player_->worldTransform_.transform_.scale.x, player_);
	collider_.SetAttribute(kCollisionAttributePlayer);
}

void PlayerContext::JustDodgeManager::DodgeExcept()
{
	// 
	dodgeTimer_.Start(justDodgeFrame_);
	// コライダーの座標
	colliderPosition_ = player_->worldTransform_.GetWorldPosition();
}

void PlayerContext::JustDodgeManager::Update()
{
	// 回避タイマー
	dodgeTimer_.Update();
	// 無敵タイマー
	invisibleTimer_.Update();

	// コライダーの更新
	collider_.Update(colliderPosition_);
}

void PlayerContext::JustDodgeManager::ImGuiDraw()
{
	static float bar = 0.01f;
	ImGui::DragFloat("Bar", &bar, 0.01f);
	ImGui::DragFloat("JustDudgeFrame", &justDodgeFrame_, bar);
}
