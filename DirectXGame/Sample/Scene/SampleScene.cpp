#include "SampleScene.h"
#include "imgui.h"
#include "Engine/2D/TextureManager.h"
#include "Engine/3D/ModelUtility/ModelManager.h"
#include "Engine/3D/ModelUtility/ModelRenderer.h"
#include "Engine/Scene/SceneManager.h"
#include "Engine/PostEffect/PostEffectRender.h"

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

	levelLoader_ = std::make_unique<LevelLoader>();
	levelLoader_->LoadSceneData("01_12");

	particles_ = std::make_unique<Particle>();
	particles_->Initialize(ModelManager::GetModel("Plane"));

	gpuParticle_ = std::make_unique<GPUParticleSystem>();
	gpuParticle_->Initialize(ModelManager::GetModel("Plane"));

	plane_.worldTransform.Initialize();
	//plane_.worldTransform.transform_.rotate.y = 3.14f;
	plane_.worldTransform.UpdateMatrix();
	plane_.model = ModelManager::GetModel("Plane");

	skyboxTransform_.Initialize();
	skyboxTransform_.transform_.translate = { 0,0,0.0f };
	skyboxTransform_.transform_.scale = { 1,1,1 };
	skyboxTransform_.UpdateMatrix();
	//---ここから書く---//

	// 初期カメラ
	//camera_.transform_.translate.y = 5.0f;
	//camera_.transform_.rotate.x = 0.4f;
	camera_.transform_.translate.z = -7.0f;
	debugCamera_ = std::make_unique<DebugCamera>();
	debugCamera_->Initialize();

	lightData_.color = { 1,1,1,1 };
	lightData_.direction = { 0,-1,0 };
	lightData_.intensity = 1.0f;

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

	hsvData_ = {};

	bloomData_ = { 0.5f,2.0f };

#pragma endregion
	// 準備完了
	isSceneReady_ = true;
	//testWTF_.transform_.translate = { 0,-3.5f,7.0f };
	//testWTF_.transform_.rotate.y = -1.85f;

	cubeMaterial_ = std::make_unique<Material>();
	cubeMaterial_->CreateMaterial();

	trail_ = std::make_unique<MissileTrail>();
	//trail_->Initialize();

	lines_ = std::make_unique<Line3D>();
	lines_->Intialize();

}

void SampleScene::GPUUpdate()
{

	gpuParticle_->Update();


}

void SampleScene::Update()
{
	// ポストエフェクトの変更
	PostChanger();

	if (input_->TriggerKey(DIK_LSHIFT)) {
		sceneManager_->ChangeScene("TITLE");
	}
	if (input_->TriggerKey(DIK_RSHIFT)) {
		sceneManager_->ChangeScene("GAME");
	}
	// コントローラー
	if (input_->XTriggerJoystick(XINPUT_GAMEPAD_B)) {
		sceneManager_->ChangeScene("GAME");
	}

	skyboxTransform_.UpdateMatrix();

	lines_->Update();

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
	desc.hsv = hsvData_;
	desc.bloom = bloomData_;
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
	ModelRenderer::PreDraw(commandList);
	// サンプル
	ModelDrawDesc desc{};
	desc.camera = &camera_;
	desc.directionalLight = directionalLight_.get();
	desc.spotLight = spotLight_.get();
	desc.pointLight = pointLight_.get();
	desc.worldTransform = &skyboxTransform_;

	skybox_->Draw(desc);

	lines_->Draw(&camera_);

	ModelRenderer::PostDraw();
	Model::PostDraw();

#pragma region スプライト

	Sprite::PreDraw(commandList);



	Sprite::PostDraw();

#pragma endregion

}

void SampleScene::UIDraw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

	Sprite::PreDraw(commandList);



	Sprite::PostDraw();
}

void SampleScene::ImGuiDraw()
{
	ImGui::Begin("SampleScene");
	ImGui::ColorEdit3("DisColor", &cubeMaterial_->dissolveColor_.x);
	ImGui::DragFloat3("DisColor", &cubeMaterial_->dissolveColor_.x, 0.01f);
	ImGui::DragFloat("dis", &cubeMaterial_->threshold_, 0.01f);
	ImGui::DragFloat3("GeneratePos", &generatePosition_.x, 0.01f);
	ImGui::DragFloat3("PlanePos", &plane_.worldTransform.transform_.translate.x, 0.01f);
	ImGui::DragFloat3("PlaneRot", &plane_.worldTransform.transform_.rotate.x, 0.01f);

	ImGui::InputInt("PostEffect", &postEffecter_, 1);


	ImGui::Checkbox("DebugCamera", &isDebugCamera_);
	ImGui::DragFloat3("modelPos", &skyboxTransform_.transform_.translate.x, 0.01f);
	ImGui::DragFloat3("modelRot", &skyboxTransform_.transform_.rotate.x, 0.01f);
	ImGui::DragFloat3("modelSca", &skyboxTransform_.transform_.scale.x, 0.01f);


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
		if (ImGui::BeginTabItem("HSV"))
		{
			ImGui::SliderFloat("HsvH", &hsvData_.hue, -1.0f, 1.0f);
			ImGui::SliderFloat("HsvS", &hsvData_.saturation, -1.0f, 1.0f);
			ImGui::SliderFloat("HsvV", &hsvData_.value, -1.0f, 1.0f);
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

	skybox_->ImGuiDraw();
	// Particle
	gpuParticle_->ImGuiDraw();
	// カメラの
	camera_.ImGuiDraw();
	debugCamera_->ImGuiDraw();

}

void SampleScene::LoadModel()
{
	ModelManager::LoadNormalModel("BarrierSphere", "sphere");
	ModelManager::LoadObjModel("Plane", "plane");
	ModelManager::LoadObjModel("Axis", "BulletTest");
	ModelManager::LoadAnimModel("AnimCube", "AnimatedCube");
	ModelManager::LoadAnimModel("Walk", "walk");
	ModelManager::LoadAnimModel("SneakWalk", "sneakWalk");
	sphere_.reset(Skydome::CreateSkydome());
	skybox_.reset(Skybox::CreateSkybox("rostock_laage_airport_4k.dds"));
}

void SampleScene::LoadTexture()
{

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

void SampleScene::PostChanger()
{
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
	if (input_->TriggerKey(DIK_9)) {
		postEffecter_ = 9;
	}

	switch (postEffecter_)
	{
	case 0:
		PostEffectRender::sPostEffect = Pipeline::PostEffectType::kAlpha;
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
	case 9:
		PostEffectRender::sPostEffect = Pipeline::PostEffectType::kBloom;
		break;
	default:
		PostEffectRender::sPostEffect = Pipeline::PostEffectType::kAlpha;
		break;
	}

}
