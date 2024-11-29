#include "JustDudgeManager.h"
#include "../../Player.h"
#include "Application/GameSystem/GameSystem.h"
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
	// 無敵時間の処理（ジャスト回避
	if (invisibleTimer_.IsActive()) {
		GameSystem::sSpeedFactor = slowFactor_;
		player_->SetColor(Vector4(1.0f, 1.0f, 1.0f, 0.3f));
	}
	if (invisibleTimer_.IsEnd()) {
		invisibleReturnTimer_.Start(45.0f);
		//GameSystem::sSpeedFactor = 1.0f;
		player_->SetColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
	}

	// 戻ってくる時間
	if (invisibleReturnTimer_.IsActive()) {
		GameSystem::sSpeedFactor = Ease::Easing(slowFactor_, 1.0f, invisibleReturnTimer_.GetElapsedFrame());
	}

	// 回避タイマー
	dodgeTimer_.Update();
	// 無敵タイマー
	invisibleTimer_.Update();
	invisibleReturnTimer_.Update();
	// コライダーの更新
	collider_.Update(colliderPosition_);
}

void PlayerContext::JustDodgeManager::ImGuiDraw()
{
	static float bar = 0.01f;
	ImGui::DragFloat("Bar", &bar, 0.01f);
	ImGui::DragFloat("JustDudgeFrame", &justDodgeFrame_, bar);
	ImGui::DragFloat("SlowFactor", &slowFactor_, bar);
}
