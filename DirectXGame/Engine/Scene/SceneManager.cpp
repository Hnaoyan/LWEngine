#include "SceneManager.h"
#include <cassert>

void SceneManager::Update()
{
	// 切り替え
	if (nextScene_ && nextScene_->GetSceneReady()) {
		if (nowScene_) {
			delete nowScene_;
		}
		// シーン切り替え
		nowScene_ = nextScene_;
		nextScene_ = nullptr;

		// シーンマネージャー設定
		nowScene_->SetSceneManager(this);
		// 次のシーンの初期化
		nowScene_->Initialize();
	}
	// 現在のシーンがあれば
	if (nowScene_) {
		// シーンのGPU更新処理
		nowScene_->GPUUpdate();
		// シーンのCPU更新処理
		nowScene_->Update();
	}
	// コマンドリストの送り出し
	DirectXCommand::ExecuteCommandList(DirectXCommand::sCommandList_.Get());
}

void SceneManager::Draw()
{
	// 現在のシーンがあれば
	if (nowScene_) {
		nowScene_->Draw();
	}
}

void SceneManager::UIDraw()
{
	// 現在のシーンがあれば
	if (nowScene_) {
		nowScene_->UIDraw();
	}
}

void SceneManager::ImGuiDraw()
{
	// 現在のシーンがあれば
	if (nowScene_) {
		nowScene_->ImGuiDraw();
	}
}

void SceneManager::ChangeScene(const std::string& sceneName)
{
	assert(sceneFactory_);
	assert(nextScene_ == nullptr);

	//if (nowScene_ == nullptr) {
	//	nextScene_ = sceneFactory_->CreateScene(sceneName);
	//	return;
	//}

	// 次のシーン生成
	nextScene_ = sceneFactory_->CreateScene(sceneName);
	// 初期化
	nextInitialize_ = std::thread(&IScene::Initialize, nextScene_);
	nextInitialize_.join();

	nextSceneName_ = sceneName;
}

void SceneManager::ChangeThreadScene(const std::string& sceneName)
{
	assert(sceneFactory_);
	assert(nextScene_ == nullptr);

	//if (nowScene_ == nullptr) {
	//	nextScene_ = sceneFactory_->CreateScene(sceneName);
	//	return;
	//}

	// 次のシーン生成
	nextScene_ = sceneFactory_->CreateScene(sceneName);
	// 初期化
	nextInitialize_ = std::thread(&IScene::Initialize, nextScene_);

	nextSceneName_ = sceneName;
}
