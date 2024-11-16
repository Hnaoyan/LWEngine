#include "KeyConfigManager.h"

KeyConfigManager::KeyConfigManager()
{
    input_ = Input::GetInstance();
}
void KeyConfigManager::Update()
{
    // キーバインド
    playerKey_.keybinds.quickBoost = XINPUT_GAMEPAD_LEFT_SHOULDER;
    playerKey_.keybinds.jump = XINPUT_GAMEPAD_A;
    playerKey_.keybinds.pressJump = input_->XRTrigger();
    playerKey_.keybinds.lockon = XINPUT_GAMEPAD_B;
    playerKey_.keybinds.shot = XINPUT_GAMEPAD_RIGHT_SHOULDER;
    playerKey_.keybinds.homingShot = XINPUT_GAMEPAD_X;
    playerKey_.keybinds.boost = XINPUT_GAMEPAD_X;

    if (isReplay_)
    {

    }
    else 
    {
        //---キー受付---//
        // 小ダッシュ
        playerKey_.keyConfigs.quickBoost = input_->XTriggerJoystick(playerKey_.keybinds.quickBoost);
        // ジャンプ
        playerKey_.keyConfigs.jump = input_->XTriggerJoystick(playerKey_.keybinds.jump);
        // 長押しジャンプ
        playerKey_.keyConfigs.pressJump = input_->XRTrigger();
        // ロックオン
        playerKey_.keyConfigs.lockon = input_->XTriggerJoystick(playerKey_.keybinds.lockon);
        // 通常射撃
        playerKey_.keyConfigs.shot = input_->XTriggerJoystick(playerKey_.keybinds.shot);
        // 追従弾
        playerKey_.keyConfigs.homingShot = input_->XTriggerJoystick(playerKey_.keybinds.homingShot);
        // ダッシュ
        playerKey_.keyConfigs.boost = input_->XTriggerJoystick(playerKey_.keybinds.boost);

        //---スティックの受付---//
        playerKey_.leftStick = input_->XGetLeftJoystick();
        playerKey_.rightStick = input_->XGetRightJoystick();

        // 保存処理
        ReplayData saveData{};
        saveData.keyConfigs = playerKey_.keyConfigs;
        saveData.leftStick = playerKey_.leftStick;
        saveData.rightStick = playerKey_.rightStick;
        //saveData.frameNumber = int32_t(nowFrame_);
        replayContainer_.push_back(saveData);
    }

    //nowFrame_++;
}
