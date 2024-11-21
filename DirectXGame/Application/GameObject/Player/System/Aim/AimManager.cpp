#include "AimManager.h"
#include "Application/GameObject/Player/Player.h"
#include "Application/GameObject/GameObjectLists.h"
#include "Engine/Camera/CameraList.h"
#include "Engine/LwLib/LwLibrary.h"
#include "Engine/2D/TextureManager.h"
#include "Engine/2D/SpriteManager.h"
#include "imgui.h"

void PlayerContext::AimManager::Initialize(Player* player)
{
	// プレイヤー
	player_ = player;
	// 入力クラス
	input_ = Input::GetInstance();
	// 初期化
	offSetTransform_.Initialize();
	// 親の設定
	parentMatrix_ = Matrix4x4::MakeAffineMatrix(player_->worldTransform_.transform_.scale, player_->camera_->transform_.rotate, player_->worldTransform_.transform_.translate);
	offSetTransform_.parentMatrix_ = &parentMatrix_;
	offSetTransform_.transform_.translate = { 0.0f,0.0f,50.0f };
	offSetTransform_.UpdateMatrix();

}

void PlayerContext::AimManager::Update(ICamera* camera)
{
	parentMatrix_ = Matrix4x4::MakeAffineMatrix(player_->worldTransform_.transform_.scale, player_->camera_->transform_.rotate, player_->worldTransform_.transform_.translate);
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
}

void PlayerContext::AimManager::ImGuiDraw()
{
	Vector3 world = offSetTransform_.GetWorldPosition();
	ImGui::DragFloat3("Transform", &offSetTransform_.transform_.translate.x);
	ImGui::DragFloat3("Wolrd", &world.x);
	ImGui::DragFloat2("ScreenPosition", &screenPosition_.x);
}
