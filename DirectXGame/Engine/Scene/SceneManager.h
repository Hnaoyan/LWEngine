#pragma once
#include<memory>

#include "IScene.h"
#include "SceneFactory/AbstractSceneFactory.h"
#include "../Utility/Singleton.h"

class SceneManager : public Singleton<SceneManager>
{
private:
	std::unique_ptr<IScene> sceneArray_[3];

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
	void SetSceneFactory(AbstractSceneFactory* sceneFactory) { sceneFactory_ = sceneFactory; }

	void Update();

	void Draw();

	void ImGuiDraw();

	void ChangeScene(const std::string& sceneName);


};
