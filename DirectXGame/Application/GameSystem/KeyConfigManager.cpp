#include "KeyConfigManager.h"

void KeyConfigManager::Update()
{
    playerKey_.keybinds_.quickBoost = XINPUT_GAMEPAD_LEFT_SHOULDER;
    playerKey_.keybinds_.jump = XINPUT_GAMEPAD_A;
    playerKey_.keybinds_.pressJump = input_->XRTrigger();
    playerKey_.keybinds_.lockon = XINPUT_GAMEPAD_B;
    playerKey_.keybinds_.shot = XINPUT_GAMEPAD_RIGHT_SHOULDER;
    playerKey_.keybinds_.homingShot = XINPUT_GAMEPAD_X;

    playerKey_.keybinds_.boost = XINPUT_GAMEPAD_X;



    // 小ダッシュ
    playerKey_.keyConfigs_.quickBoost = input_->XTriggerJoystick(playerKey_.keybinds_.quickBoost);
    // ジャンプ
    playerKey_.keyConfigs_.jump = input_->XTriggerJoystick(playerKey_.keybinds_.jump);
    // 長押しジャンプ
    playerKey_.keyConfigs_.pressJump = input_->XRTrigger();
    // ロックオン
    playerKey_.keyConfigs_.lockon = input_->XTriggerJoystick(playerKey_.keybinds_.lockon);
    // 通常射撃
    playerKey_.keyConfigs_.shot = input_->XTriggerJoystick(playerKey_.keybinds_.shot);
    // 追従弾
    playerKey_.keyConfigs_.homingShot = input_->XTriggerJoystick(playerKey_.keybinds_.homingShot);

    // 
    playerKey_.keyConfigs_.boost = input_->XTriggerJoystick(playerKey_.keybinds_.boost);
}
