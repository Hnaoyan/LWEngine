#include "FrameWork.h"
#include "../3D/Graphics/Shader.h"

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
	//Shader::GetInstance()->Initialize();


}

void Framework::Finalize()
{


	CoUninitialize();
}

void Framework::Update()
{
}
