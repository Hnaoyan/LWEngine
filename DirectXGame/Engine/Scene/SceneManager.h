#pragma once
#include<memory>

#include "IScene.h"
#include "SceneFactory/AbstractSceneFactory.h"
#include "../Utility/Singleton.h"

class SceneManager : public Singleton<SceneManager>
{
private:
	//std::unique_ptr<IScene> sceneArray_[3];

	int sceneNum_ = 0;
	int prevSceneNum_ = 0;

	/// <summary>
	/// シーンファクトリー
	/// </summary>
	AbstractSceneFactory* sceneFactory_ = nullptr;

	// 現在のシーン
	IScene* nowScene_ = nullptr;
	// 次のシーン
	IScene* nextScene_ = nullptr;
	// 名前
	std::string nextSceneName_{};

public:
	/// <summary>
	/// シーン生成クラス設定
	/// </summary>
	/// <param name="sceneFactory"></param>
	void SetSceneFactory(AbstractSceneFactory* sceneFactory) { sceneFactory_ = sceneFactory; }
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 3D描画
	/// </summary>
	void Draw();
	/// <summary>
	/// 2D・UI描画
	/// </summary>
	void UIDraw();
	/// <summary>
	/// ImGui
	/// </summary>
	void ImGuiDraw();
	/// <summary>
	/// シーン変更
	/// </summary>
	/// <param name="sceneName"></param>
	void ChangeScene(const std::string& sceneName);


};
