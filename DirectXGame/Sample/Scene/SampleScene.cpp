#include "SampleScene.h"
#include "imgui.h"
#include "../../Engine/2D/TextureManager.h"

void SampleScene::Initialize()
{
	// 基底クラス初期化
	IScene::Initialize();
	// モデル読み込み
	LoadModel();
	// テクスチャ関係読み込み
	LoadTexture();

	directionalLight_.reset(DirectionalLight::CreateLight());

	testWTF_.Initialize();
	testWTF_.transform_.translate = { 0,0,0.0f };
	testWTF_.transform_.scale = { 1,1,1 };
	testWTF_.UpdateMatrix();
	//---ここから書く---//

	//sampleObj_ = std::make_unique<AnimSampleObject>();
	//sampleObj_->Initialize(this->testModel_.get(),cubeModel_.get());
	//sampleObj_->worldTransform_.transform_.translate = { -2.5f,0,0 };

	// 歩くオブジェ
	walkObj_ = std::make_unique<AnimSampleObject>();
	walkObj_->Initialize(this->walkModel_.get(),cubeModel_.get());
	walkObj_->worldTransform_.transform_.translate = { 0,0,0 };
	walkObj_->worldTransform_.transform_.rotate = { 0,3.14f,0 };

	cubeObj_ = std::make_unique<AnimCubeObject>();
	cubeObj_->Initialize(testModel_.get());
	cubeObj_->worldTransform_.transform_.translate = { -5.0f,0,10.0f };

	player_ = std::make_unique<PlSampleObject>();
	player_->Initialize(testModel_.get());

	// 初期カメラ
	camera_.transform_.translate.z = -5.0f;
	debugCamera_ = std::make_unique<DebugCamera>();
	debugCamera_->Initialize();

	lightData_.color = { 1,1,1,1 };
	lightData_.direction = { 0,-1,0 };
	lightData_.intensity = 1.0f;

	newSpriteData_.spriteTransform_ = {
		{1.0f,1.0f,1.0f},
		{0,0,0},
		{0,0,0},
	};
	newSpriteData_.isInvisible_ = true;
}

void SampleScene::Update()
{
	newSprite_->SetPosition(newSpriteData_.position_);
	newSprite_->SetUVTransform(newSpriteData_.spriteTransform_);
	newSprite_->SetInvisible(newSpriteData_.isInvisible_);

	testWTF_.UpdateMatrix();
	//sampleObj_->Update();
	walkObj_->Update();
	cubeObj_->Update();
	player_->Update();
	// カメラの更新
	CameraUpdate();

	directionalLight_->Update(lightData_);
}

void SampleScene::Draw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();
#pragma region スプライト
		
	Sprite::PreDraw(commandList);


	Sprite::PostDraw();

#pragma endregion
	// 深度クリア
	dxCommon_->ClearDepthBuffer();

	Model::PreDraw(commandList);

	// サンプル
	//sampleObj_->Draw(&camera_);
	ModelDrawDesc desc{};
	desc.camera = &camera_;
	desc.directionalLight = directionalLight_.get();
	walkObj_->Draw(desc);
	//player_->Draw(&camera_);
	cubeObj_->Draw(desc);
	desc.worldTransform = &testWTF_;
	sphere_->Draw(desc);

	skybox_->Draw(desc);

	Model::PostDraw();

#pragma region スプライト

	Sprite::PreDraw(commandList);

	newSprite_->Draw();

	Sprite::PostDraw();

#pragma endregion

}

void SampleScene::ImGuiDraw()
{
	ImGui::Begin("SampleScene");
	ImGui::Checkbox("DebugCamera", &isDebugCamera_);
	if (ImGui::TreeNode("SpriteData")) {
		ImGui::Checkbox("Invisible", &newSpriteData_.isInvisible_);
		ImGui::DragFloat2("pos", &newSpriteData_.position_.x, 0.01f);
		ImGui::DragFloat3("SpriteTPos", &newSpriteData_.spriteTransform_.translate.x, 0.01f);
		ImGui::DragFloat3("SpriteTRot", &newSpriteData_.spriteTransform_.rotate.x, 0.01f);
		ImGui::DragFloat3("SpriteTSca", &newSpriteData_.spriteTransform_.scale.x, 0.01f);
		ImGui::TreePop();
	}

	ImGui::DragFloat3("modelPos", &testWTF_.transform_.translate.x, 0.01f);
	ImGui::DragFloat3("modelRot", &testWTF_.transform_.rotate.x, 0.01f);
	ImGui::DragFloat3("modelSca", &testWTF_.transform_.scale.x, 0.01f);


	if (ImGui::TreeNode("Lighting")) {
		ImGui::ColorEdit4("Color", &lightData_.color.x);
		ImGui::DragFloat3("Direction", &lightData_.direction.x, 0.01f);
		lightData_.direction = Vector3::Normalize(lightData_.direction);
		ImGui::DragFloat("Intensity", &lightData_.intensity, 0.01f);
		ImGui::TreePop();
	}

	ImGui::End();
	ImGui::ShowDemoWindow();

	//sampleObj_->ImGuiDraw();

	player_->ImGuiDraw();
	skybox_->ImGuiDraw();
	walkObj_->ImGuiDraw();
	cubeObj_->ImGuiDraw();
	// カメラの
	camera_.ImGuiDraw();
	debugCamera_->ImGuiDraw();

	ImGui::Begin("Scene");
	int srvIndex = dxCommon_->GetSrvHandler()->sNextDescriptorNum_;
	ImGui::InputInt("srvIndex", &srvIndex);
	ImGui::End();

}

void SampleScene::LoadModel()
{
	testModel_.reset(Model::CreateObj("AnimatedCube", LoadExtension::kGltf));
	walkModel_.reset(Model::CreateObj("walk", LoadExtension::kGltf));

	cubeModel_.reset(Model::CreateDefault("cube"));

	sphere_.reset(Sphere::CreateSphere());

	skybox_.reset(Skybox::CreateSkybox("rostock_laage_airport_4k.dds"));
}

void SampleScene::LoadTexture()
{
	newSpriteData_.uvTexture_ = TextureManager::Load("Resources/default/uvChecker.png");
	newSprite_.reset(Sprite::Create(newSpriteData_.uvTexture_, { 300.0f,0.0f }, { 0,0 }));
}

void SampleScene::CameraUpdate()
{
	debugCamera_->Update();

	if (isDebugCamera_) {
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
