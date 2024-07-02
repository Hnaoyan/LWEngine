#include "AimManager.h"
#include "../Player.h"
#include "../../../../Engine/Camera/CameraList.h"
#include "../../../../Engine/LwLib/LwEngineLib.h"
#include "../../../../Engine/2D/TextureManager.h"
#include "imgui.h"

void AimManager::Initialize(Player* player)
{
	// プレイヤー
	player_ = player;
	// 入力クラス
	input_ = Input::GetInstance();

	if (reticleSprite_ == nullptr) {
		uint32_t texture = TextureManager::Load("Resources/crossHair.png");
		reticleSprite_.reset(Sprite::Create(texture, { 0,0 }, { 0.5f,0.5f }));
	}
	
	// 初期化
	offSetTransform_.Initialize();
	// 親の設定
	offSetTransform_.parent_ = &player_->worldTransform_;
	// 初手の奥行き
	offSetTransform_.transform_.translate.z = 50.0f;
	offSetTransform_.UpdateMatrix();

	reset_.isReceivingInput = false;
	reset_.rate = 0.03f;
}

void AimManager::Update(ICamera* camera)
{
	// エイムの位置変更
	AimUpdate();

	reticleSprite_->SetPosition(screenPosition_);

	// 座標の更新
	// スクリーン座標の更新
	screenPosition_ = LwLib::WorldToScreen(offSetTransform_.GetWorldPosition(), camera);
	// オフセットの座標更新
	offSetTransform_.UpdateMatrix();
}

void AimManager::ImGuiDraw()
{
	Vector3 world = offSetTransform_.GetWorldPosition();
	ImGui::DragFloat3("LocalPosition", &offSetTransform_.transform_.translate.x);
	ImGui::DragFloat3("WolrdPosition", &world.x);
	ImGui::DragFloat2("ScreenPosition", &screenPosition_.x);
}

void AimManager::Draw()
{
	reticleSprite_->Draw();
}

void AimManager::AimUpdate()
{
	XINPUT_STATE joyState;
	// 移動速度
	float reticleSpeed = 1.0f;
	if (input_->GetJoystickState(0, joyState)) {
		// 右スティックの動かす処理
		offSetTransform_.transform_.translate.x += (float)joyState.Gamepad.sThumbRX / SHRT_MAX * reticleSpeed;
		offSetTransform_.transform_.translate.y += (float)joyState.Gamepad.sThumbRY / SHRT_MAX * reticleSpeed;

		float limit = 20.0f;
		if (offSetTransform_.transform_.translate.x > limit) {
			offSetTransform_.transform_.translate.x = limit;
		}
		else if (offSetTransform_.transform_.translate.x < -limit) {
			offSetTransform_.transform_.translate.x = -limit;
		}

		if (offSetTransform_.transform_.translate.y > limit) {
			offSetTransform_.transform_.translate.y = limit;
		}
		else if (offSetTransform_.transform_.translate.y < -limit) {
			offSetTransform_.transform_.translate.y = -limit;
		}

		// 入力中か
		if ((float)joyState.Gamepad.sThumbRX != 0 || (float)joyState.Gamepad.sThumbRY != 0) {
			reset_.isReceivingInput = true;
		}
		else {
			reset_.isReceivingInput = false;
		}
	}

	if (!reset_.isReceivingInput) {
		offSetTransform_.transform_.translate.x = LwLib::Lerp(offSetTransform_.transform_.translate.x, 0, reset_.rate);
		offSetTransform_.transform_.translate.y = LwLib::Lerp(offSetTransform_.transform_.translate.y, 0, reset_.rate);
	}

}
