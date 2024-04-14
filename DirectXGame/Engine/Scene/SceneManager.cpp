#include "SceneManager.h"
#include <cassert>

void SceneManager::Update()
{
	// 切り替え
	if (nextScene_) {
		if (nowScene_) {
			delete nowScene_;
		}
		// シーン切り替え
		nowScene_ = nextScene_;
		nextScene_ = nullptr;
		// 次のシーンの初期化
		nowScene_->Initialize();
	}
	// シーン更新処理
	nowScene_->Update();
}

void SceneManager::Draw()
{
	nowScene_->Draw();
}

void SceneManager::ChangeScene(const std::string& sceneName)
{
	assert(sceneFactory_);
	assert(nextScene_ == nullptr);

	if (nowScene_ == nullptr) {
		nextScene_ = sceneFactory_->CreateScene(sceneName);
		return;
	}

	// 次のシーン生成
	nextSceneName_ = sceneName;
}
