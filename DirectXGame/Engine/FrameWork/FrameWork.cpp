#include "FrameWork.h"

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
	winApp_ = WindowAPI::GetInstance();
	winApp_->CreateGameWindow();

}

void Framework::Finalize()
{


	CoUninitialize();
}

void Framework::Update()
{
}
