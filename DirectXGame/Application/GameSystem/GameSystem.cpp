#include "GameSystem.h"
#include "Engine/Input/Input.h"
#include "Engine/LwLib/LwEngineLib.h"

float GameSystem::sSpeedFactor = 1.0f;
GameSystem::PlayerKeyConfig GameSystem::sPlayerKey;
GameSystem::DashBlur GameSystem::sBlurEffect;

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
    bloomData_ = { 0.75f,1.5f };
}

void GameSystem::KeyBindUpdate()
{
    sPlayerKey.keybinds_.quickBoost = XINPUT_GAMEPAD_LEFT_SHOULDER;
    sPlayerKey.keybinds_.shot = XINPUT_GAMEPAD_RIGHT_SHOULDER;
    sPlayerKey.keybinds_.jump = XINPUT_GAMEPAD_A;
    sPlayerKey.keybinds_.lockon = XINPUT_GAMEPAD_B;
    sPlayerKey.keybinds_.boost = XINPUT_GAMEPAD_X;

    sPlayerKey.keybinds_.pressJump = input_->XRTrigger();
}

void GameSystem::KeyConfigUpdate()
{
    sPlayerKey.keyConfigs_.quickBoost = input_->XTriggerJoystick(sPlayerKey.keybinds_.quickBoost);
    sPlayerKey.keyConfigs_.jump = input_->XTriggerJoystick(sPlayerKey.keybinds_.jump);
    sPlayerKey.keyConfigs_.lockon = input_->XTriggerJoystick(sPlayerKey.keybinds_.lockon);
    sPlayerKey.keyConfigs_.shot = input_->XTriggerJoystick(sPlayerKey.keybinds_.shot);
    sPlayerKey.keyConfigs_.boost = input_->XTriggerJoystick(sPlayerKey.keybinds_.boost);

    sPlayerKey.keyConfigs_.pressJump = input_->XRTrigger();
}

void GameSystem::Update()
{
    // バインドの更新
    KeyBindUpdate();
    // 入力の更新
    KeyConfigUpdate();

    PostEffectRender::PostEffectDesc desc{};
    desc.blur = sBlurEffect.data;
    PostEffectRender::GetInstance()->bloom_.cMap_->threshold = bloomData_.threshold;
    PostEffectRender::GetInstance()->bloom_.cMap_->sigma = bloomData_.sigma;
    //PostEffectRender::GetInstance()->Update(desc);

}
