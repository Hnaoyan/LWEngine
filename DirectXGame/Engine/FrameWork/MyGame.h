#pragma once
#include "Framework.h"
#include "../Scene/SceneManager.h"

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


};

