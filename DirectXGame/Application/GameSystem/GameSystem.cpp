#include "GameSystem.h"
#include "Engine/Input/Input.h"
#include "Engine/LwLib/LwEngineLib.h"

float GameSystem::sSpeedFactor = 1.0f;
GameSystem::PlayerKeyConfig GameSystem::sPlayerKey;

float GameSystem::GameSpeedFactor()
{
    if (sSpeedFactor <= 1.0f) {
        sSpeedFactor = 1.0f;
    }
    return (1.0f / sSpeedFactor) * kDeltaTime;
}

void GameSystem::Initialize()
{
    input_ = Input::GetInstance();
    // バインドの設定
    KeyBindUpdate();
}

void GameSystem::KeyBindUpdate()
{
    sPlayerKey.keybinds_.dash = XINPUT_GAMEPAD_LEFT_SHOULDER;
    sPlayerKey.keybinds_.jump = XINPUT_GAMEPAD_A;
    sPlayerKey.keybinds_.lockon = XINPUT_GAMEPAD_B;
    sPlayerKey.keybinds_.shot = XINPUT_GAMEPAD_RIGHT_SHOULDER;
}

void GameSystem::KeyConfigUpdate()
{
    sPlayerKey.keyConfigs_.dash = input_->XTriggerJoystick(sPlayerKey.keybinds_.dash);
    sPlayerKey.keyConfigs_.jump = input_->XTriggerJoystick(sPlayerKey.keybinds_.jump);
    sPlayerKey.keyConfigs_.lockon = input_->XTriggerJoystick(sPlayerKey.keybinds_.lockon);
    sPlayerKey.keyConfigs_.shot = input_->XTriggerJoystick(sPlayerKey.keybinds_.shot);
}

void GameSystem::Update()
{
    // バインドの更新
    KeyBindUpdate();
    // 入力の更新
    KeyConfigUpdate();
}
