#pragma once
#include "TerrainCluster.h"
#include <vector>

class TerrainManager
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model);
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="desc"></param>
	void Draw(ModelDrawDesc desc);

	/// <summary>
	/// 追加
	/// </summary>
	void AddCluster();

	/// <summary>
	/// ImGui
	/// </summary>
	void ImGuiDraw();

private:
	// モデル
	Model* model_ = nullptr;
	// グループのリスト
	std::vector<std::unique_ptr<InstancedGroup>> clusters_;
};
