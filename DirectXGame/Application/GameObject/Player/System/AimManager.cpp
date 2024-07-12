#include "AimManager.h"
#include "../Player.h"
#include "Engine/Camera/CameraList.h"
#include "Engine/LwLib/LwEngineLib.h"
#include "Engine/2D/TextureManager.h"
#include "../../GameObjectLists.h"
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
	// オフセットの座標更新
	offSetTransform_.UpdateMatrix();
	// ターゲットがいる場合ターゲットにAIM
	if (player_->GetOperation()->GetLockOn()->ExistTarget()) {
		screenPosition_ = LwLib::WorldToScreen(player_->GetOperation()->GetLockOn()->GetTarget()->worldTransform_.GetWorldPosition(), camera);
		targetPosition_ = player_->GetOperation()->GetLockOn()->GetTarget()->worldTransform_.GetWorldPosition();
	}
	// ターゲットがいない場合オフセットに
	else {
		screenPosition_ = LwLib::WorldToScreen(offSetTransform_.GetWorldPosition(), camera);
		targetPosition_ = offSetTransform_.GetWorldPosition();
	}
	// クロスヘアの座標設定
	reticleSprite_->SetPosition(screenPosition_);
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
