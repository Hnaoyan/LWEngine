#pragma once
#include <vector>
#include <memory>

// 敵
class SampleEnemy;
class Player;
class ICamera;

/// <summary>
/// ロックオン処理を管理する
/// </summary>
class LockOn
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="player"></param>
	void Initialize(Player* player);
	/// <summary>
	/// 更新
	/// </summary>
	void Update(ICamera* camera);
public:
	// リストの設定
	void SetEnemyList(std::vector<std::unique_ptr<SampleEnemy>>* lists) { enemys_ = lists; }
	// ターゲットのポインタ
	SampleEnemy* GetTarget() { return target_; }

private:
	// プレイヤー
	Player* player_ = nullptr;
	// リスト
	std::vector<std::unique_ptr<SampleEnemy>>* enemys_;
	// ターゲット
	SampleEnemy* target_ = nullptr;
};