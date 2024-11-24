#include "IScene.h"
#include "SceneManager.h"

void IScene::Initialize()
{
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();

	// カメラ初期化
	camera_.Initialize();
}
