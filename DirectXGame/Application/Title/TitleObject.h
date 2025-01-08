#pragma once
#include "../GameObject/IGameObject.h"

class TitleObject : public IGameObject
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model"></param>
	void Initialize(Model* model) override;
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;
	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="desc"></param>
	void Draw(ModelDrawDesc desc) override;
	/// <summary>
	/// ImGui描画
	/// </summary>
	/// <param name="name"></param>
	void ImGuiDraw() override;



};
