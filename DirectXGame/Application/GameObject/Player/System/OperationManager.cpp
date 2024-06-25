#include "OperationManager.h"
#include "../../../GameSystem/GameSystem.h"
#include "../../../../Engine/Input/Input.h"
#include "../Player.h"

void OparationManager::Initialize(Player* player)
{
	assert(player);
	// プレイヤー
	player_ = player;
	// 入力
	input_ = Input::GetInstance();
}

void OparationManager::Update()
{
	// 入力
	InputUpdate();

}

void OparationManager::InputUpdate()
{
	XINPUT_STATE joyState;
	float speed = 4.0f;
	if (input_->GetJoystickState(0, joyState)) {
		Vector3 direct = { (float)joyState.Gamepad.sThumbLX / SHRT_MAX,(float)joyState.Gamepad.sThumbLY / SHRT_MAX ,0 };
		player_->worldTransform_.transform_.translate.x += (direct.x * GameSystem::GameSpeedFactor() * speed);
		player_->worldTransform_.transform_.translate.z += (direct.y * GameSystem::GameSpeedFactor() * speed);
		if ((direct.x != 0 || direct.y != 0) && joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A)
		{
			player_->worldTransform_.transform_.translate.x += (direct.x * GameSystem::GameSpeedFactor() * 20.0f);
			player_->worldTransform_.transform_.translate.z += (direct.y * GameSystem::GameSpeedFactor() * 20.0f);
		}
	}
}
