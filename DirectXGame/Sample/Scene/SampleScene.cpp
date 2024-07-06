#include "SampleScene.h"
#include "imgui.h"
#include "../../Engine/2D/TextureManager.h"
#include "../../Engine/Scene/SceneManager.h"
#include "../../Engine/3D/ModelManager.h"
#include "../../Engine/PostEffect/PostEffectRender.h"

void SampleScene::Initialize()
{
	// 基底クラス初期化
	IScene::Initialize();
	// ライティング初期化
	LightingInitialize();
	// モデル読み込み
	LoadModel();
	// テクスチャ関係読み込み
	LoadTexture();

	testWTF_.Initialize();
	testWTF_.transform_.translate = { 0,0,0.0f };
	testWTF_.transform_.scale = { 1,1,1 };
	testWTF_.UpdateMatrix();
	//---ここから書く---//

	//sampleObj_ = std::make_unique<AnimSampleObject>();
	//sampleObj_->Initialize(this->testModel_.get(),cubeModel_.get());
	//sampleObj_->worldTransform_.transform_.translate = { -2.5f,0,0 };

	// 歩くオブジェ
	humans_[0] = std::make_unique<AnimSampleObject>();
	humans_[0]->Initialize(this->walkModel_.get(),cubeModel_);
	humans_[0]->worldTransform_.transform_.translate = { 0,0,0 };
	humans_[0]->worldTransform_.transform_.rotate = { 0,3.14f,0 };

	humans_[1] = std::make_unique<AnimSampleObject>();
	humans_[1]->Initialize(this->walkModel_.get(), cubeModel_);
	humans_[1]->worldTransform_.transform_.translate = { 0,0,0 };
	humans_[1]->worldTransform_.transform_.rotate = { 0,3.14f,0 };

	humans_[2] = std::make_unique<AnimSampleObject>();
	humans_[2]->Initialize(this->sneakWalkModel_.get(), cubeModel_);
	humans_[2]->worldTransform_.transform_.translate = { 0,0,0 };
	humans_[2]->worldTransform_.transform_.rotate = { 0,3.14f,0 };

	cubes_[0] = std::make_unique<AnimCubeObject>();
	cubes_[0]->Initialize(testModel_.get());
	//cubeObj_->worldTransform_.transform_.translate = { -5.0f,0,10.0f };

	cubes_[1] = std::make_unique<AnimCubeObject>();
	cubes_[1]->Initialize(testModel_.get());

	player_ = std::make_unique<PlSampleObject>();
	player_->Initialize(testModel_.get());

	// 初期カメラ
	camera_.transform_.translate.y = 5.0f;
	camera_.transform_.rotate.x = 0.4f;
	camera_.transform_.translate.z = -7.0f;
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

	TextureManager::sEnvironmentTexture = skybox_->GetTexture();

#pragma region PostEffect
	vignetteData_.scale = 16.0f;
	vignetteData_.powValue = 0.8f;
	vignetteData_.color = { 1.0f,0.0f,0.0f };

	blurData_.centerPoint = { 0.5f,0.5f };
	blurData_.samplesNum = 5;
	blurData_.blurWidth = 0.01f;

	dissolveData_.color = {};
	dissolveData_.threshold = 0;

#pragma endregion


	//testWTF_.transform_.translate = { 0,-3.5f,7.0f };
	//testWTF_.transform_.rotate.y = -1.85f;
}

void SampleScene::Update()
{

	if (input_->TriggerKey(DIK_RSHIFT)) {
		sceneManager_->ChangeScene("GAME");
	}
	// コントローラー
	XINPUT_STATE joyState;
	if (input_->GetJoystickState(0, joyState)) {
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A) {
			sceneManager_->ChangeScene("GAME");
		}
	}

	if (input_->TriggerKey(DIK_0)) {
		postEffecter_ = 0;
	}
	if (input_->TriggerKey(DIK_1)) {
		postEffecter_ = 1;
	}
	if (input_->TriggerKey(DIK_2)) {
		postEffecter_ = 2;
	}
	if (input_->TriggerKey(DIK_3)) {
		postEffecter_ = 3;
	}
	if (input_->TriggerKey(DIK_4)) {
		postEffecter_ = 4;
	}
	if (input_->TriggerKey(DIK_5)) {
		postEffecter_ = 5;
	}
	if (input_->TriggerKey(DIK_6)) {
		postEffecter_ = 6;
	}
	if (input_->TriggerKey(DIK_7)) {
		postEffecter_ = 7;
	}
	if (input_->TriggerKey(DIK_8)) {
		postEffecter_ = 8;
	}


	newSprite_->SetPosition(newSpriteData_.position_);
	newSprite_->SetUVTransform(newSpriteData_.spriteTransform_);
	newSprite_->SetInvisible(newSpriteData_.isInvisible_);

	testWTF_.UpdateMatrix();

	humans_[0]->Update();
	humans_[1]->Update();
	humans_[2]->Update();

	for (int i = 0; i < cubes_.size(); ++i) {
		cubes_[i]->Update();
	}
	player_->Update();
	// カメラの更新
	CameraUpdate();

	directionalLight_->Update(lightData_);
	spotLight_->Update(spLightData_);
	pointLight_->Update(ptLightData_);

	PostEffectRender::PostEffectDesc desc{};
	desc.blur = blurData_;
	desc.vignette = vignetteData_;
	desc.noise = noiseData_;
	desc.dissolve = dissolveData_;
	PostEffectRender::GetInstance()->Update(desc);

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
	desc.spotLight = spotLight_.get();
	desc.pointLight = pointLight_.get();
	desc.worldTransform = &testWTF_;

	humans_[0]->Draw(desc);
	humans_[1]->Draw(desc);
	humans_[2]->Draw(desc);

	//player_->Draw(&camera_);
	for (int i = 0; i < cubes_.size(); ++i) {
		cubes_[i]->Draw(desc);
	}
	ModelDrawDesc textureDesc{};
	textureDesc.camera = &camera_;
	textureDesc.directionalLight = directionalLight_.get();
	textureDesc.spotLight = spotLight_.get();
	textureDesc.pointLight = pointLight_.get();
	textureDesc.texture = newSpriteData_.uvTexture_;
	textureDesc.worldTransform = &testWTF_;
	cubeModel_->Draw(textureDesc);
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
	ImGui::InputInt("PostEffect", &postEffecter_, 1);

	switch (postEffecter_)
	{
	case 0:
		PostEffectRender::sPostEffect = Pipeline::PostEffectType::kNormal;
		break;
	case 1:
		PostEffectRender::sPostEffect = Pipeline::PostEffectType::kGrayScale;
		break;
	case 2:
		PostEffectRender::sPostEffect = Pipeline::PostEffectType::kVignette;
		break;
	case 3:
		PostEffectRender::sPostEffect = Pipeline::PostEffectType::kGrayscaleVignette;
		break;
	case 4:
		PostEffectRender::sPostEffect = Pipeline::PostEffectType::kGaussian;
		break;
	case 5:
		PostEffectRender::sPostEffect = Pipeline::PostEffectType::kSmoothing;
		break;
	case 6:
		PostEffectRender::sPostEffect = Pipeline::PostEffectType::kRadialBlur;
		break;
	case 7:
		PostEffectRender::sPostEffect = Pipeline::PostEffectType::kDissolve;
		break;
	case 8:
		PostEffectRender::sPostEffect = Pipeline::PostEffectType::kNoise;
		break;
	default:
		PostEffectRender::sPostEffect = Pipeline::PostEffectType::kNormal;
		break;
	}

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


	if (ImGui::BeginTabBar("PostEffect"))
	{
		float defaultSpeed = 0.01f;
		if (ImGui::BeginTabItem("Vignette"))
		{
			ImGui::ColorEdit3("ViColor", &vignetteData_.color.x);
			ImGui::DragFloat("ViPowValue", &vignetteData_.powValue, defaultSpeed);
			ImGui::DragFloat("ViScale", &vignetteData_.scale, defaultSpeed);
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Blur"))
		{
			ImGui::DragFloat2("BlColor", &blurData_.centerPoint.x, defaultSpeed);
			ImGui::DragFloat("BlBlurWidth", &blurData_.blurWidth, defaultSpeed);
			ImGui::InputInt("BlSampleNum", &blurData_.samplesNum, 1);
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Noise"))
		{
			bool isEnable = noiseData_.enableScreen;
			ImGui::Checkbox("Screen", &isEnable);
			noiseData_.enableScreen = isEnable;
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Dissolve"))
		{
			ImGui::ColorEdit3("DiColor", &dissolveData_.color.x);
			ImGui::DragFloat("DiThreshold", &dissolveData_.threshold, 0.01f, 0.0f, 1.0f);
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
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

	player_->ImGuiDraw();
	skybox_->ImGuiDraw();
	humans_[0]->ImGuiDraw();
	humans_[1]->ImGuiDraw();
	humans_[2]->ImGuiDraw();
	for (int i = 0; i < cubes_.size(); ++i) {
		cubes_[i]->ImGuiDraw();
	}
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
	sneakWalkModel_.reset(Model::CreateObj("sneakWalk", LoadExtension::kGltf));
	//cubeModel_.reset(Model::CreateDefault("terrain"));
	cubeModel_ = ModelManager::GetModel("DefaultCube");
	sphere_.reset(Skydome::CreateSkydome());

	skybox_.reset(Skybox::CreateSkybox("rostock_laage_airport_4k.dds"));
}

void SampleScene::LoadTexture()
{
	newSpriteData_.uvTexture_ = TextureManager::Load("Resources/default/white2x2.png");
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

void SampleScene::LightingInitialize()
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
