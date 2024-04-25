#include "FrameWork.h"
#include "../3D/Graphics/Shader.h"
#include "../3D/Graphics/GraphicsPSO.h"

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

	// シェーダー読み込みクラス初期化
	Shader::GetInstance()->Initialize();
	// パイプラインの初期化
	GraphicsPSO::GetInstance()->Initialize(dxCommon_->GetDevice());

	// ImGuiの生成
	imGuiManager_ = std::make_unique<ImGuiManager>();
	imGuiManager_->Initialize(dxCommon_, winApp_);

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
