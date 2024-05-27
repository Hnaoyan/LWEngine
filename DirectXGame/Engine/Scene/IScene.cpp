#include "IScene.h"

void IScene::Initialize()
{
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();

	// カメラ初期化
	camera_.Initialize();
}

void IScene::CameraUpdate()
{
	// カメラ更新
	camera_.Update();
}
