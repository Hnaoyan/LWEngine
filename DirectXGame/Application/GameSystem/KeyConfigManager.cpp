#include "KeyConfigManager.h"
#include "Replay/ReplayManager.h"

KeyConfigManager::KeyConfigManager()
{
    input_ = Input::GetInstance();
    // キーバインドの設定
    playerKey_.keybinds.quickBoost = XINPUT_GAMEPAD_LEFT_SHOULDER;
    playerKey_.keybinds.jump = XINPUT_GAMEPAD_A;
    playerKey_.keybinds.pressJump = input_->XRTrigger();
    playerKey_.keybinds.lockon = XINPUT_GAMEPAD_B;
    playerKey_.keybinds.shot = XINPUT_GAMEPAD_RIGHT_SHOULDER;
    playerKey_.keybinds.homingShot = XINPUT_GAMEPAD_X;
    playerKey_.keybinds.boost = XINPUT_GAMEPAD_X;

}
void KeyConfigManager::Update(ReplayManager* replayManager)
{

    if (isReplay_)
    {
        // データの取得
        playerKey_.keyConfigs = replayManager->GetReplayData(playFrame_).keyConfigs;
        playerKey_.leftStick = replayManager->GetReplayData(playFrame_).leftStick;
        playerKey_.rightStick = replayManager->GetReplayData(playFrame_).rightStick;

        // カウント
        playFrame_++;
        if (playFrame_ >= replayManager->GetReplayDataSize()) {
            isReplay_ = false;
        }
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
    }
}

void KeyConfigManager::BeginReplay()
{
    isReplay_ = true;
    playFrame_ = 0;
}
