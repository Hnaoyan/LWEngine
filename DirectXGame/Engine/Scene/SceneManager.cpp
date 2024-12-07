#include "SceneManager.h"
#include "Transition/SceneTransitionManager.h"
#include <cassert>
#include <imgui.h>

void SceneManager::Update()
{
	// 切り替え
	if (nextScene_ && nextScene_->GetSceneReady()) {
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

		transitionManager_->ExecuteReturn();

		// シーンマネージャー設定
		nowScene_->SetSceneManager(this);
		// 次のシーンの初期化
		//nowScene_->Initialize();
	}
	// 現在のシーンがあれば
	if (nowScene_) {
		// シーンの更新処理
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
	// シーンマネージャ
	ImGui::Begin("SceneManager");
	ImGui::ColorEdit4("TransitionColor", &transitionManager_->sColor.x);
	ImGui::End();
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
	//// 遷移開始
	//transitionManager_->ExecuteStart(120.0f);
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
	nextInitialize_ = std::thread(&IScene::LoadResource, nextScene_);
	isThread_ = true;
	nextSceneName_ = sceneName;
	isChangeActive_ = true;
	// 遷移開始
	transitionManager_->ExecuteStart(120.0f);
}
