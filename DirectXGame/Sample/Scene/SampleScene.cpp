#include "SampleScene.h"
#include "imgui.h"
#include "Engine/2D/TextureManager.h"
#include "Engine/2D/SpriteManager.h"
#include "Engine/3D/ModelUtility/ModelManager.h"
#include "Engine/3D/ModelUtility/ModelRenderer.h"
#include "Engine/Scene/SceneManager.h"
#include "Engine/PostEffect/PostEffectRender.h"
#include "Engine/LwLib/LwLibLists.h"

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

	planeMaterial_ = std::make_unique<Material>();
	planeMaterial_->CreateMaterial();

	trail_ = std::make_unique<MissileTrail>();
	//trail_->Initialize();

	lines_ = std::make_unique<Line3D>();
	lines_->Intialize();

	trailPolygon_ = std::make_unique<Trail3D>();

	Vector3 point = {};
	for (int i = 0; i < 8; i++) {
		curvePoints_.push_back(point);
		point += Vector3(1.0f, 0.0f, 0.0f);
	}
	trailPolygon_->texture_ = TextureManager::Load("Resources/default/uvChecker.png");
	trailPolygon_->SetMaxWidth(5.0f);
	trailPolygon_->SetMinWidth(5.0f);
	trailPolygon_->SetCamera(&camera_);
	//curvePoints_[0] = { 0.0f,0.0f,0.0f };
	//curvePoints_[1] = { 3.0f,3.0f,0.0f };
	//curvePoints_[2] = { 5.0f,0.0f,0.0f };
	//curvePoints_[3] = { 2.0f,-3.0f,0.0f };

}

void SampleScene::GPUUpdate()
{

	gpuParticle_->Update();


}

void SampleScene::Update()
{
	// ポストエフェクトの変更
	PostChanger();

#ifdef IMGUI_ENABLED
	if (input_->TriggerKey(DIK_LEFTARROW)) {
		sceneManager_->ChangeScene("TITLE");
}
	if (input_->TriggerKey(DIK_RIGHTARROW)) {
		sceneManager_->ChangeScene("GAME");
	}
	// コントローラー
	if (input_->XTriggerJoystick(XINPUT_GAMEPAD_B)) {
		sceneManager_->ChangeScene("GAME");
	}
#endif // IMGUI_ENABLE

	planeMaterial_->Update();
	planeMaterial_->enableLighting_ = 0;
	skyboxTransform_.UpdateMatrix();

	// キャトムルのコンテナ
	std::vector<Vector3> sts;
	// 始点
	sts.push_back(LwLib::Curve::CatmullRomSpline(curvePoints_[0], curvePoints_[1], curvePoints_[2], curvePoints_[3], 0.0f));
	// 補間点
	for (int i = 1; i <= 10; ++i) {
		float ratio = (float)i / 10;
		Vector3 newPoint = LwLib::Curve::CatmullRomSpline(curvePoints_[0], curvePoints_[1], curvePoints_[2], curvePoints_[3], ratio);
		sts.push_back(newPoint);
	}

	int vtIndex = 0;
	for (int i = 0; i < 10; ++i) {
		lines_->vertexData_[vtIndex++].position = sts[i];
		lines_->vertexData_[vtIndex++].position = sts[i + 1];
	}

	lines_->vertexData_[vtIndex++].position = curvePoints_[1];
	lines_->vertexData_[vtIndex++].position = curvePoints_[2];

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

	skyboxTransform_.transform_.scale.y = skyboxTransform_.transform_.scale.x;

	// 板ポリ
	DrawDesc::ModelDesc modelDesc{};
	modelDesc.material = planeMaterial_.get();
	modelDesc.mesh = planeModel_->GetMesh();
	modelDesc.modelData = planeModel_->GetModelData();
	modelDesc.texture = TextureManager::Load("Resources/Effect/effect.png");
	modelDesc.worldTransform = &skyboxTransform_;
	DrawDesc::LightDesc lightDesc{};
	lightDesc.directionalLight = directionalLight_.get();
	lightDesc.pointLight = pointLight_.get();
	lightDesc.spotLight = spotLight_.get();
	ModelRenderer::NormalDraw(&camera_, modelDesc, lightDesc);

	//ModelRenderer::LineDraw(&camera_, lines_.get());
	//ModelRenderer::TriangleDraw(&camera_, triangle_.get());
	//ModelRenderer::TrailDraw(&camera_, trailPolygon_.get());

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

	SpriteManager::GetSprite("UVChecker")->SetUVTransform(uvTransform_);
	//SpriteManager::GetSprite("UVChecker")->Draw();

	circleSprite_->SetColor(color_);
	effectSprite_->SetColor(color_);

	//circleSprite_->Draw();
	//effectSprite_->Draw();

	Sprite::PostDraw();
}

void SampleScene::ImGuiDraw()
{
	std::vector<Vector3> interpolatedPoints;
	if (isCurve_) {
		if (curvePoints_.size() > 8) {
			for (int i = 1; i < curvePoints_.size() - 2; ++i) {
				for (float t = 0.0f; t <= 1.0f; t += (1.0f / curveInter_)) {
					t = std::clamp(t, 0.0f, 1.0f);
					interpolatedPoints.push_back(LwLib::Curve::CatmullRomSpline(curvePoints_[i - 1], curvePoints_[i], curvePoints_[i + 1], curvePoints_[i + 2], t));
				}
			}
		}
		else {
			interpolatedPoints = curvePoints_;
		}
	}
	else {
		interpolatedPoints = curvePoints_;
	}

	trailPolygon_->LerpWidthVertex(interpolatedPoints);
	trailPolygon_->Update();
	//triangle_->Update(curvePoints_);

	ImGui::Begin("SampleScene");
	ImGui::ColorEdit4("PlaneColor", &planeMaterial_->color_.x);
	if (ImGui::TreeNode("EffectSprite")) {
		ImGui::ColorEdit4("EffectColor", &color_.x);
		if (ImGui::Button("None")) {
			circleSprite_->SetBlendMode(Pipeline::BlendMode::kNone);
			effectSprite_->SetBlendMode(Pipeline::BlendMode::kNone);
		}
		if (ImGui::Button("Alpha")) {
			circleSprite_->SetBlendMode(Pipeline::BlendMode::kAlpha);
			effectSprite_->SetBlendMode(Pipeline::BlendMode::kAlpha);
		}
		if (ImGui::Button("Add")) {
			circleSprite_->SetBlendMode(Pipeline::BlendMode::kAdd);
			effectSprite_->SetBlendMode(Pipeline::BlendMode::kAdd);
		}
		if (ImGui::Button("Sub")) {
			circleSprite_->SetBlendMode(Pipeline::BlendMode::kSubtract);
			effectSprite_->SetBlendMode(Pipeline::BlendMode::kSubtract);
		}
		if (ImGui::Button("Mul")) {
			circleSprite_->SetBlendMode(Pipeline::BlendMode::kMultiply);
			effectSprite_->SetBlendMode(Pipeline::BlendMode::kMultiply);
		}
		if (ImGui::Button("Scr")) {
			circleSprite_->SetBlendMode(Pipeline::BlendMode::kScreen);
			effectSprite_->SetBlendMode(Pipeline::BlendMode::kScreen);
		}

		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Trail")) {
		ImGui::DragFloat3("uvPos", &uvTransform_.translate.x, 0.01f);
		ImGui::DragFloat3("uvSca", &uvTransform_.scale.x, 0.01f);
		ImGui::DragFloat3("uvRot", &uvTransform_.rotate.x, 0.01f);
		trailPolygon_->GetMaterial()->SetUVTransform(uvTransform_);
		if (ImGui::TreeNode("Texture")) {
			ImGui::InputText("TexturePath", &texturePath_, 256);
			std::string texturePath = "Resources/";
			texturePath += texturePath_;

			if (ImGui::Button("LoadTexture")) {
				trailPolygon_->texture_ = TextureManager::Load("Resources/Effect/effect.png");
			}
			if (ImGui::Button("BlackTrail")) {
				trailPolygon_->texture_ = TextureManager::Load("Resources/Effect/BlackTrail.png");
			}
			if (ImGui::Button("WhiteTrail")) {
				trailPolygon_->texture_ = TextureManager::Load("Resources/Effect/WhiteTrail.png");
			}
			if (ImGui::Button("WhitePNG")) {
				trailPolygon_->texture_ = TextureManager::Load("Resources/default/white2x2.png");
			}
			if (ImGui::Button("GN")) {
				trailPolygon_->texture_ = TextureManager::Load("Resources/default/GNPalse.png");
			}
			ImGui::TreePop();
		}

		ImGui::Checkbox("IsCurve", &isCurve_);
		ImGui::DragFloat("Inter", &curveInter_, 0.1f);
		ImGui::DragFloat3("GeneratePos", &generatePosition_.x, 0.01f);
		if (ImGui::Button("Generate")) {
			curvePoints_.push_back(generatePosition_);
		}
		std::string name;
		int num = 0;
		if (ImGui::TreeNode("Curve")) {
			for (auto it = curvePoints_.begin(); it != curvePoints_.end(); ++it) {
				name = "CurvePoint" + std::to_string(num);
				ImGui::DragFloat3(name.c_str(), &(*it).x, 0.01f);
				ImGui::Separator();
				num++;
			}
			ImGui::TreePop();
		}
		ImGui::TreePop();
	}

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
	ModelManager::LoadAnimModel("AnimCube", "AnimatedCube");
	ModelManager::LoadAnimModel("Walk", "walk");
	ModelManager::LoadAnimModel("SneakWalk", "sneakWalk");
	ModelManager::LoadObjModel("TestPlane", "plane");
	planeModel_ = ModelManager::GetModel("TestPlane");
	sphere_.reset(Skydome::CreateSkydome());
	skybox_.reset(Skybox::CreateSkybox("rostock_laage_airport_4k.dds"));
}

void SampleScene::LoadTexture()
{
	SpriteManager::LoadSprite("UVChecker", TextureManager::Load("Resources/default/uvChecker.png"));
	SpriteManager::GetSprite("UVChecker")->SetPosition({ 1280 / 2,720 / 2 });

	SpriteManager::LoadSprite("CirclePNG", TextureManager::Load("Resources/effect/circle.png"));
	SpriteManager::LoadSprite("EffectPNG", TextureManager::Load("Resources/effect/Frea.png"));
	effectSprite_ = SpriteManager::GetSprite("CirclePNG");
	circleSprite_ = SpriteManager::GetSprite("EffectPNG");

	effectSprite_->SetPosition({ 1280 / 2,100 });
	circleSprite_->SetPosition({ 1280 / 2,500 });

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
