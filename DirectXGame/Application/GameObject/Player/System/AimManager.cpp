#include "AimManager.h"
#include "../Player.h"
#include "../../../../Engine/LwLib/LwEngineLib.h"

void AimManager::Initialize(Player* player)
{
	// プレイヤー
	player_ = player;
	// 入力クラス
	input_ = Input::GetInstance();
	
	// 初期化
	offSetTransform_.Initialize();
	// 親の設定
	offSetTransform_.parent_ = &player_->worldTransform_;
	// 初手の奥行き
	offSetTransform_.transform_.translate.z = 50.0f;
}

void AimManager::Update(ICamera* camera)
{
	// エイムの位置変更
	AimUpdate();

	// 座標の更新
	// スクリーン座標の更新
	screenPosition_ = LwLib::WorldToScreen(offSetTransform_.GetWorldPosition(), camera);
	// オフセットの座標更新
	offSetTransform_.UpdateMatrix();
}

void AimManager::ImGuiDraw()
{

}

void AimManager::AimUpdate()
{
	XINPUT_STATE joyState;
	// 移動速度
	float reticleSpeed = 1.0f;
	if (input_->GetJoystickState(0, joyState)) {
		// 右スティックの動かす処理
		offSetTransform_.transform_.translate.x += (float)joyState.Gamepad.sThumbLX / SHRT_MAX * reticleSpeed;
		offSetTransform_.transform_.translate.y += (float)joyState.Gamepad.sThumbLY / SHRT_MAX * reticleSpeed;
	}
}
