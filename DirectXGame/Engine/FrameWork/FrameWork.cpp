#include "FrameWork.h"
#include "Engine/Audio/Audio.h"
#include "../3D/Graphics/Shader.h"
#include "../3D/Graphics/GraphicsPSO.h"
#include "../2D/Drawer/Sprite.h"

void Framework::Execute()
{
	// 初期化
	Initialize();

	// ゲームループ
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

	// 入力クラス
	input_ = Input::GetInstance();
	input_->Initialize();

	// シェーダー読み込みクラス初期化
	Shader::GetInstance()->Initialize();
	// パイプラインの初期化
	GraphicsPSO::GetInstance()->Initialize(dxCommon_->GetDevice());

	// 生成
	imGuiManager_ = std::make_unique<ImGuiManager>();
	textureManager_ = std::make_unique<TextureManager>();
	modelManager_ = std::make_unique<ModelManager>();
	spriteManager_ = std::make_unique<SpriteManager>();
	sceneManager_ = std::make_unique<SceneManager>();

	// ImGuiの初期化
	imGuiManager_->Initialize(dxCommon_, winApp_);

	// テクスチャマネージャの初期化
	TextureManager::GetInstance()->Initialize(dxCommon_);
	TextureManager::Load("Resources/default/white2x2.png");

	// モデルマネージャの初期化
	modelManager_->LoadNormalModel("DefaultCube", "cube");
	modelManager_->LoadNormalModel("Sphere", "SphereCollider");
	modelManager_->LoadNormalModel("Box", "BoxCollider");
	modelManager_->LoadNormalModel("Plane", "plane");

	// スプライトの初期化
	Sprite::StaticInitialize(dxCommon_->GetDevice(), WindowAPI::kWindowWidth, WindowAPI::kWindowHeight);

	// オーディオの初期化
	Audio::GetInstance()->Initialize();	
}

void Framework::Finalize()
{
	Audio::GetInstance()->Finalize();
	dxCommon_->Finalize();
	imGuiManager_->Finalize();
	CoUninitialize();
}

void Framework::Update()
{
}
