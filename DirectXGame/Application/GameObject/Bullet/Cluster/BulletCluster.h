#pragma once
#include "../IBulletCluster.h"

/// <summary>
/// 弾の集合体クラス
/// </summary>
class BulletCluster : public IBulletCluster
{
public: // 仮想関数
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

};