#include "Player.h"
#include "imgui.h"

void Player::Initialize(Model* model)
{
	// 基底クラスの初期化
	IGameObject::Initialize(model);

	worldPosition_ = worldTransform_.GetWorldPosition();
	// システム関係の初期化
	SystemInitialize();

}

void Player::Update()
{
	// システム関係の更新
	SystemUpdate();

	// ワールド座標を設定
	worldTransform_.transform_.translate = worldPosition_;
	// 基底クラスの更新
	IGameObject::Update();
}

void Player::Draw(ModelDrawDesc desc)
{
	ModelDrawDesc drawDesc{};
	// クラスの値設定
	drawDesc.worldTransform = &worldTransform_;
	// 引数の値設定
	drawDesc.camera = desc.camera;
	drawDesc.directionalLight = desc.directionalLight;
	drawDesc.pointLight = desc.pointLight;
	drawDesc.spotLight = desc.spotLight;
	// プレイヤーの描画
	model_->Draw(drawDesc);
}

void Player::ImGuiDraw()
{
	std::string name = "Player";
	ImGui::Begin(name.c_str());

	ImGui::DragFloat3("Velocity", &velocity_.x);

	// システムのタブ
	if (ImGui::BeginTabBar("System"))
	{
		if (ImGui::BeginTabItem("Aim")) {
			aimManager_.ImGuiDraw();
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("S2")) {
			
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("S3")) {

			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}

	ImGui::End();
}

void Player::OnCollision(ColliderObject target)
{
	target;
}

void Player::UISpriteDraw()
{
	aimManager_.Draw();
}

void Player::SystemInitialize()
{
	// 操作システム
	systemManager_.Initialize(this);
	// エイムシステム
	aimManager_.Initialize(this);
}

void Player::SystemUpdate()
{
	systemManager_.Update();

	aimManager_.Update(camera_);
}
