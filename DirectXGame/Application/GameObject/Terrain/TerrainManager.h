#pragma once
#include "TerrainCluster.h"
#include <vector>

class CollisionManager;

/// <summary>
/// 地形マネージャ
/// </summary>
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
	void AddCluster(std::string tag);
	void AddCluster(std::string filename, std::string modelTag);

	/// <summary>
	/// ImGui
	/// </summary>
	void ImGuiDraw();

	/// <summary>
	/// コリジョン登録処理
	/// </summary>
	/// <param name="manager"></param>
	void CollisionUpdate(CollisionManager* manager);

private:
	// モデル
	Model* model_ = nullptr;
	// グループのリスト
	std::vector<std::unique_ptr<InstancedGroup>> clusters_;
	// 隠すフラグ
	bool isInvisible_ = false;

};
