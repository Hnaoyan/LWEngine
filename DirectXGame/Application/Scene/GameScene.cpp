#include "GameScene.h"
#include <imgui.h>

void GameScene::Initialize()
{
	// 基底クラス初期化
	IScene::Initialize();
	// ライト初期化
	LightingInitialize();
	// モデル読み込み
	LoadModel();
	// テクスチャ関係読み込み
	LoadTexture();



#pragma region カメラ関係
	// 初期カメラ
	camera_.transform_.translate.y = 5.0f;
	camera_.transform_.rotate.x = 0.4f;
	camera_.transform_.translate.z = -7.0f;
	debugCamera_ = std::make_unique<DebugCamera>();
	debugCamera_->Initialize();
	followCamera_ = std::make_unique<FollowCamera>();
	followCamera_->Initialize();
#pragma endregion

}

void GameScene::Update()
{
	// カメラの更新
	CameraUpdate();
}

void GameScene::Draw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();
#pragma region 背景

	Sprite::PreDraw(commandList);



	Sprite::PostDraw();

#pragma endregion
	// 深度クリア
	dxCommon_->ClearDepthBuffer();

	Model::PreDraw(commandList);

	ModelDrawDesc desc{};
	desc.camera = &camera_;
	desc.directionalLight = directionalLight_.get();
	desc.pointLight = pointLight_.get();
	desc.spotLight = spotLight_.get();

	Model::PostDraw();

#pragma region UI

	Sprite::PreDraw(commandList);


	Sprite::PostDraw();

#pragma endregion
}

void GameScene::ImGuiDraw()
{
	ImGui::Begin("SampleScene");
	ImGui::Checkbox("DebugCamera", &isDebugCamera_);

	if (ImGui::TreeNode("DirectionalLight")) {
		ImGui::TreePop();
	}
	if (ImGui::BeginTabBar("Lighting"))
	{
		float defaultSpeed = 0.01f;
		if (ImGui::BeginTabItem("DirectionalLight"))
		{
			ImGui::ColorEdit4("Color", &lightData_.color.x);
			ImGui::DragFloat3("Direction", &lightData_.direction.x, defaultSpeed);
			lightData_.direction = Vector3::Normalize(lightData_.direction);
			ImGui::DragFloat("Intensity", &lightData_.intensity, defaultSpeed);
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("PointLight"))
		{
			ImGui::ColorEdit4("ptColor", &ptLightData_.color.x);
			ImGui::DragFloat("ptDecay", &ptLightData_.decay, defaultSpeed);
			ImGui::DragFloat("ptIntensity", &ptLightData_.intensity, defaultSpeed);
			ImGui::DragFloat("ptRadius", &ptLightData_.radius, defaultSpeed);
			ImGui::DragFloat3("ptPosition", &ptLightData_.position.x, defaultSpeed);
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("SpotLight"))
		{
			ImGui::ColorEdit4("spColor", &spLightData_.color.x);
			ImGui::DragFloat("spDecay", &spLightData_.decay, defaultSpeed);
			ImGui::DragFloat("spIntensity", &spLightData_.intensity, defaultSpeed);
			ImGui::DragFloat("spCosAngle", &spLightData_.cosAngle, defaultSpeed);
			ImGui::DragFloat("spCosFalloffStart", &spLightData_.cosFalloffStart, defaultSpeed);
			ImGui::DragFloat("spDistance", &spLightData_.distance, defaultSpeed);
			ImGui::DragFloat3("spPosition", &spLightData_.position.x, defaultSpeed);
			ImGui::DragFloat3("spDirection", &spLightData_.direction.x, defaultSpeed);
			spLightData_.direction = Vector3::Normalize(spLightData_.direction);
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}

	ImGui::End();
	// カメラ
	camera_.ImGuiDraw();
	debugCamera_->ImGuiDraw();
	followCamera_->ImGuiDraw();
}

void GameScene::LoadModel()
{
	// モデルのロード

}

void GameScene::LoadTexture()
{
	// テクスチャのロード

}

void GameScene::CameraUpdate()
{

	if (isDebugCamera_) {
		debugCamera_->Update();
		followCamera_->Update();
		camera_.viewMatrix_ = debugCamera_->viewMatrix_;
		camera_.projectionMatrix_ = debugCamera_->projectionMatrix_;
		camera_.TransferMatrix();
		//camera_.Update();
	}
	else {
		// 基底更新
		IScene::CameraUpdate();
	}
}

void GameScene::LightingInitialize()
{
	// ライト作成
	directionalLight_.reset(DirectionalLight::CreateLight());
	pointLight_.reset(PointLight::CreateLight());
	spotLight_.reset(SpotLight::CreateLight());

	// 点光源データ
	ptLightData_.intensity = 1.0f;
	ptLightData_.position = { 0,2.0f,0 };
	ptLightData_.color = { 1,1,1,1 };
	ptLightData_.decay = 1.0f;
	ptLightData_.radius = 2.0f;

	// 照光源データ
	spLightData_.color = { 1,1,1,1 };
	spLightData_.position = { 2.0f,1.25f,0.0f };
	spLightData_.distance = 7.0f;
	spLightData_.direction = Vector3::Normalize({ -1.0f,-1.0f,0.0f });
	spLightData_.intensity = 4.0f;
	spLightData_.decay = 2.0f;
	spLightData_.cosAngle = std::cosf(std::numbers::pi_v<float> / 3.0f);
}
