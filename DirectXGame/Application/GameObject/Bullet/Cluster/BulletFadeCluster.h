#pragma once
#include "../IBulletCluster.h"

#include <optional>

/// <summary>
/// 消える弾の集合（現在使ってない
/// </summary>
class BulletFadeCluster : public IBulletCluster
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

public:
	// 削除申請
	std::optional<bool> deleteRequest_;
private:

};
