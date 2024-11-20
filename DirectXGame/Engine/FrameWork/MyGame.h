#pragma once
#include "Framework.h"
#include "Engine/GlobalVariables/GlobalVariables.h"
#include "Engine/Scene/SceneManager.h"
#include "Engine/Scene/SceneFactory/AbstractSceneFactory.h"

class MyGame : public Framework
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 終了
	/// </summary>
	void Finalize() override;

	/// <summary>
	/// 毎フレーム更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

	/// <summary>
	/// ImGui
	/// </summary>
	void ImGuiDraw() override;

private:
	std::unique_ptr<AbstractSceneFactory> sceneFactory_;
};

