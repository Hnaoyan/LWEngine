#include "OperationManager.h"
#include "../../../GameSystem/GameSystem.h"
#include "../Player.h"
#include "../../Bullet/SampleBulletManager.h"

#include "Engine/Input/Input.h"
#include "Engine/LwLib/LwEngineLib.h"
#include "Engine/PostEffect/PostEffectRender.h"

void OparationManager::Initialize(Player* player)
{
	assert(player);
	// プレイヤー
	player_ = player;
	// 入力
	input_ = Input::GetInstance();
	// ロックオン
	lockOn_.Initialize(player);
	// Aim
	aimManager_.Initialize(player);
}

void OparationManager::Update()
{
	// 入力
	InputUpdate();
	// ターゲットが死んだ場合解除するための更新
	lockOn_.Update();
	// Aimの処理
	aimManager_.Update(player_->camera_);
	// クールタイム
	shotTimer_.Update(GameSystem::sSpeedFactor);

}

void OparationManager::InputUpdate()
{
	// コントローラー操作
	// 方向取得
	// ジャンプ入力
	if (input_->XTriggerJoystick(XINPUT_GAMEPAD_A) && player_->velocity_.y == 0.0f)
	{
		player_->GetVerticalState()->ChangeRequest(VerticalStates::kJump);

	}
		
	if (input_->XRTrigger() && !shotTimer_.isActive_) {
		SampleBulletManager::GenerateData data{};
		data.position = player_->worldTransform_.GetWorldPosition();
		data.velocity = Vector3::Normalize(aimManager_.GetWorldPosition() - player_->worldTransform_.GetWorldPosition());
		float speedValue = 30.0f;
		data.velocity *= speedValue;
		bulletManager_->AddBullet(data);
		shotTimer_.Start(30.0f);
	}

	if (input_->XTriggerJoystick(XINPUT_GAMEPAD_B)) {
		lockOn_.ToggleLockOn(player_->camera_);
	}

	Vector2 direct = input_->XGetLeftJoystick();

	bool isDash = std::holds_alternative<QuickBoostState*>(player_->GetHorizontalState()->GetNowState());

	if ((direct.x != 0 || direct.y != 0) && !isDash) {
		if (input_->XTriggerJoystick(XINPUT_GAMEPAD_LEFT_SHOULDER)) {
			player_->GetHorizontalState()->ChangeRequest(HorizontalStates::kQuickBoost);
		}
	}
	//
}
