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


	testWTF_.Initialize();
	testWTF_.transform_.translate = { 0,0,0.0f };
	testWTF_.transform_.scale = { 0.2f,0.2f,0.2f };
	testWTF_.UpdateMatrix();
	//---ここから書く---//

	//sampleObj_ = std::make_unique<AnimSampleObject>();
	//sampleObj_->Initialize(this->testModel_.get(),cubeModel_.get());
	//sampleObj_->worldTransform_.transform_.translate = { -2.5f,0,0 };

	// 歩くオブジェ
	walkObj_ = std::make_unique<AnimSampleObject>();
	walkObj_->Initialize(this->walkModel_.get(),cubeModel_.get());
	walkObj_->worldTransform_.transform_.translate = { 10.0f,0,0 };

	cubeObj_ = std::make_unique<AnimCubeObject>();
	cubeObj_->Initialize(testModel_.get());
	cubeObj_->worldTransform_.transform_.translate = { 0,0,0 };

	// 初期カメラ
	camera_.transform_.translate.z = -5.0f;

}

void SampleScene::Update()
{
	newSprite_->SetPosition(position_);

	//sampleObj_->Update();
	walkObj_->Update();
	cubeObj_->Update();
	// カメラの更新
	CameraUpdate();
}

void SampleScene::Draw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();
#pragma region スプライト
		
	Sprite::PreDraw(commandList);

	//newSprite_->Draw();

	Sprite::PostDraw();

#pragma endregion
	// 深度クリア
	dxCommon_->ClearDepthBuffer();

	Model::PreDraw(commandList);

	// サンプル
	//sampleObj_->Draw(&camera_);
	walkObj_->Draw(&camera_);

	cubeObj_->Draw(&camera_);
	//ModelDrawDesc desc;
	//desc.worldTransform = &testWTF_;
	//desc.camera = &camera_;
	//sphere_->Draw(desc);

	Model::PostDraw();

#pragma region スプライト

	Sprite::PreDraw(commandList);


	Sprite::PostDraw();

#pragma endregion

}

void SampleScene::ImGuiDraw()
{
	//ImGui::Begin("Test");
	//ImGui::DragFloat2("pos", &position_.x, 0.01f);

	//ImGui::DragFloat3("modelPos", &testWTF_.transform_.translate.x, 0.01f);
	//ImGui::DragFloat3("modelRot", &testWTF_.transform_.rotate.x, 0.01f);
	//ImGui::DragFloat3("modelSca", &testWTF_.transform_.scale.x, 0.01f);

	//ImGui::End();
	ImGui::ShowDemoWindow();

	//sampleObj_->ImGuiDraw();
	walkObj_->ImGuiDraw();
	cubeObj_->ImGuiDraw();
	// カメラの
	camera_.ImGuiDraw();

	ImGui::Begin("Scene");
	int srvIndex = dxCommon_->GetSrvHandler()->sNextDescriptorNum_;
	ImGui::InputInt("srvIndex", &srvIndex);
	ImGui::End();

}

void SampleScene::LoadModel()
{
	testModel_.reset(Model::CreateObj("AnimatedCube", LoadExtension::kGltf));
	walkModel_.reset(Model::CreateObj("sneakWalk", LoadExtension::kGltf));

	cubeModel_.reset(Model::CreateDefault("cube"));

	sphere_.reset(Sphere::CreateSphere());

}

void SampleScene::LoadTexture()
{
	uvTexture_ = TextureManager::Load("Resources/default/uvChecker.png");
	newSprite_.reset(Sprite::Create(uvTexture_, { 300.0f,0.0f }, { 0,0 }));
}

void SampleScene::CameraUpdate()
{
	// 基底更新
	IScene::CameraUpdate();
}
