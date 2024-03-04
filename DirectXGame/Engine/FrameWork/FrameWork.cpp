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
		
		// 描画
		Draw();

	}

	// 終了処理
	Finalize();
}

void Framework::Initialize()
{
}

void Framework::Finalize()
{
}

void Framework::Update()
{
}
