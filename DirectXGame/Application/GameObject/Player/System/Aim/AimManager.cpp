#include "AimManager.h"
#include "Application/GameObject/Player/Player.h"
#include "Application/GameObject/GameObjectLists.h"
#include "Engine/Camera/CameraList.h"
#include "Engine/LwLib/LwEngineLib.h"
#include "Engine/2D/TextureManager.h"
#include "Engine/2D/SpriteManager.h"
#include "imgui.h"

void AimManager::Initialize(Player* player)
{
	// プレイヤー
	player_ = player;
	// 入力クラス
	input_ = Input::GetInstance();

	//if (reticleSprite_ == nullptr) {
	//	uint32_t texture = TextureManager::Load("Resources/crossHair.png");
	//	reticleSprite_.reset(Sprite::Create(texture, { 0,0 }, { 0.5f,0.5f }));
	//}
	reticleSprite_ = SpriteManager::GetSprite("CrossHair");
	// 初期化
	offSetTransform_.Initialize();
	// 親の設定
	parentMatrix_ = Matrix4x4::MakeAffineMatrix(player_->worldTransform_.transform_.scale, player_->camera_->transform_.rotate, player_->worldTransform_.transform_.translate);
	offSetTransform_.parentMatrix_ = &parentMatrix_;
	offSetTransform_.transform_.translate = { 0.0f,0.0f,50.0f };
	// 初手の奥行き
	//offsetPosition_ = { 0,0,50.0f };
	//offSetTransform_.transform_.translate = player_->worldTransform_.GetWorldPosition() + offsetPosition_;
	offSetTransform_.UpdateMatrix();

	reset_.isReceivingInput = false;
	reset_.rate = 0.03f;
}

void AimManager::Update(ICamera* camera)
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
		// カメラの前方向ベクトルを計算
		float frontX = std::cosf(player_->camera_->transform_.rotate.x) * std::cosf(player_->camera_->transform_.rotate.y);
		float frontY = std::sinf(player_->camera_->transform_.rotate.x);
		float frontZ = std::cosf(player_->camera_->transform_.rotate.x) * std::sinf(player_->camera_->transform_.rotate.y);
		// ピッチ角（Y軸方向）を求めるために atan2 を使用
		float pitchAngle = std::atan2f(frontY, sqrt(frontX * frontX + frontZ * frontZ));
		Vector3 normalPosition = { offSetTransform_.GetWorldPosition().x,offSetTransform_.GetWorldPosition().y + (pitchAngle * -1.0f),offSetTransform_.GetWorldPosition().z };
		screenPosition_ = LwLib::WorldToScreen(normalPosition, camera);
		targetPosition_ = normalPosition;
	}
	// クロスヘアの座標設定
	reticleSprite_->SetPosition(screenPosition_);
}

void AimManager::ImGuiDraw()
{
	Vector3 world = offSetTransform_.GetWorldPosition();
	ImGui::DragFloat3("Transform", &offSetTransform_.transform_.translate.x);
	ImGui::DragFloat3("Local", &offsetPosition_.x, 0.01f);
	ImGui::DragFloat3("Wolrd", &world.x);
	ImGui::DragFloat2("ScreenPosition", &screenPosition_.x);
}

void AimManager::Draw()
{
	reticleSprite_->Draw();
}
