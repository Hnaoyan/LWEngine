#include "SceneManager.h"
#include <cassert>

void SceneManager::Update()
{
	// 切り替え
	if (nextScene_ && nextScene_->GetSceneReady()) {
		//nextInitialize_.detach();
		if (nowScene_) {
			delete nowScene_;
		}
		if (isThread_) {
			nextInitialize_.join();
			nextScene_->Initialize();
		}
		isThread_ = false;
		// シーン切り替え
		nowScene_ = nextScene_;
		nextScene_ = nullptr;
		isChangeActive_ = false;

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
	if (isChangeActive_) {
		return;
	}
	assert(sceneFactory_);
	assert(nextScene_ == nullptr);

	// 次のシーン生成
	nextScene_ = sceneFactory_->CreateScene(sceneName);
	// 初期化
	nextInitialize_ = std::thread(&IScene::Initialize, nextScene_);
	nextInitialize_.join();

	nextSceneName_ = sceneName;
	isChangeActive_ = true;
}

void SceneManager::ChangeThreadScene(const std::string& sceneName)
{
	if (isChangeActive_) {
		return;
	}
	assert(sceneFactory_);
	assert(nextScene_ == nullptr);

	// 次のシーン生成
	nextScene_ = sceneFactory_->CreateScene(sceneName);
	// 初期化
	//nextInitialize_ = std::thread(&IScene::Initialize, nextScene_);
	nextInitialize_ = std::thread(&IScene::LoadResource, nextScene_);
	isThread_ = true;
	nextSceneName_ = sceneName;

	isChangeActive_ = true;
}
