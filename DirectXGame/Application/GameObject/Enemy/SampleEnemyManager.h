#pragma once
#include "SampleEnemy.h"

class SampleEnemyManager
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model"></param>
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

private:
	// 全部のモデル
	Model* model_ = nullptr;
	// 弾のリスト
	std::vector<std::unique_ptr<SampleEnemy>> enemys_;


};
