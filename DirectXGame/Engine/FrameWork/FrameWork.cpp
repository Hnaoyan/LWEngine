#include "FrameWork.h"
#include "../3D/Graphics/Shader.h"
#include "../3D/Graphics/GraphicsPSO.h"
#include "../2D/Drawer/Sprite.h"

void Framework::Execute()
{
	// 初期化
	Initialize();

	while (true)
	{
		// 更新
		Update();

		// 終了分岐
		if (endRequest_) {
			break;
		}

		// 描画
		Draw();

	}

	// 終了処理
	Finalize();
}

void Framework::Initialize()
{
	// WindowAPI
	winApp_ = WindowAPI::GetInstance();
	winApp_->CreateGameWindow();

	// DirectX
	dxCommon_ = DirectXCommon::GetInstance();
	dxCommon_->Initialize(winApp_);

	input_ = Input::GetInstance();
	input_->Initialize();

	// シェーダー読み込みクラス初期化
	Shader::GetInstance()->Initialize();
	// パイプラインの初期化
	GraphicsPSO::GetInstance()->Initialize(dxCommon_->GetDevice());

	// ImGuiの生成
	imGuiManager_ = std::make_unique<ImGuiManager>();
	imGuiManager_->Initialize(dxCommon_, winApp_);

	// テクスチャマネージャの初期化
	textureManager_ = std::make_unique<TextureManager>();
	TextureManager::GetInstance()->Initialize(dxCommon_);
	textureManager_->Load("Resources/default/white2x2.png");
	textureManager_->Load("Resources/default/uvChecker.png");
	textureManager_->Load("Resources/Dissolve/noise0.png");
	textureManager_->Load("Resources/Dissolve/noise1.png");
	// モデルマネージャの初期化
	modelManager_ = std::make_unique<ModelManager>();
	modelManager_->LoadNormalModel("DefaultCube", "cube");

	Sprite::StaticInitialize(dxCommon_->GetDevice(), WindowAPI::kWindowWidth, WindowAPI::kWindowHeight);

	// シーンマネージャーの生成
	sceneManager_ = std::make_unique<SceneManager>();
	
}

void Framework::Finalize()
{

	imGuiManager_->Finalize();
	CoUninitialize();
}

void Framework::Update()
{
}
