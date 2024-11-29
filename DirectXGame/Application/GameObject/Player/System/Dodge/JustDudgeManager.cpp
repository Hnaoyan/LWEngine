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
	// 回避受付開始
	dodgeTimer_.Start(justDodgeFrame_);
	// コライダーの座標
	colliderPosition_ = player_->worldTransform_.GetWorldPosition();
}

void PlayerContext::JustDodgeManager::InvisibleExcept(const float& frame)
{
	// スローモーション（無敵時間）開始処理
	invisibleTimer_.Start(frame);
	float keepFrame = 60.0f * 5.0f;
	comboData_.keepTimer.Start(keepFrame);
	comboData_.nowCombo++;

	// 最大値に行ったら
	if (comboData_.nowCombo >= comboData_.maxCombo) {
		comboData_.nowCombo = comboData_.maxCombo;
		// 攻撃の変化
		player_->GetSystemFacede()->GetShootingManager()->SetAttackPattern(true);
	}
}

void PlayerContext::JustDodgeManager::Update()
{
	// 無敵時間の処理（ジャスト回避
	if (invisibleTimer_.IsActive()) {
		GameSystem::sSpeedFactor = slowFactor_;
		player_->SetColor(Vector4(1.0f, 1.0f, 1.0f, 0.3f));
	}
	// 戻り始める処理
	if (invisibleTimer_.IsEnd()) {
		invisibleReturnTimer_.Start(45.0f);
		//GameSystem::sSpeedFactor = 1.0f;
		player_->SetColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
	}
	// 戻ってくる時間
	if (invisibleReturnTimer_.IsActive()) {
		GameSystem::sSpeedFactor = Ease::Easing(slowFactor_, 1.0f, invisibleReturnTimer_.GetElapsedFrame());
	}

	if (comboData_.keepTimer.IsActive()) {
		// コンボ
		switch (comboData_.nowCombo)
		{
		case 0:
			player_->SetColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
			break;
		case 1:
			player_->SetColor(Vector4(1.0f, 1.0f, 0.0f, 0.7f));
			break;
		case 2:
			player_->SetColor(Vector4(1.0f, 0.0f, 0.0f, 0.7f));
			break;
		case 3:
			player_->SetColor(Vector4(0.0f, 1.0f, 0.0f, 0.7f));
			break;
		case 4:
			player_->SetColor(Vector4(0.0f, 0.0f, 1.0f, 0.7f));
			break;
		case 5:
			player_->SetColor(Vector4(0.0f, 1.0f, 1.0f, 1.0f));
			break;
		default:
			break;
		}

		//Vector3 playerColor = { player_->GetMaterial()->color_.x,player_->GetMaterial()->color_.y,player_->GetMaterial()->color_.z };
		//Vector3 color = Ease::Easing(playerColor, Vector3(1.0f, 1.0f, 1.0f), comboData_.keepTimer.GetElapsedFrame());
		//player_->SetColor(Vector4(color.x, color.y, color.z, player_->GetMaterial()->color_.w));
	}
	if (comboData_.keepTimer.IsEnd()) {
		player_->SetColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
	}

	// コンボ維持タイマー
	comboData_.keepTimer.Update();
	// コンボリセット
	if (comboData_.keepTimer.IsEnd()) {
		comboData_.nowCombo = 0;
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
	ImGui::InputInt("ComboCount", &comboData_.nowCombo);
}
