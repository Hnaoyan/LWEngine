#pragma once
#include <vector>
#include <memory>

// 敵
class SampleEnemy;
class Player;

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

	void SetEnemyList(std::vector<std::unique_ptr<SampleEnemy>>* lists) { enemys_ = lists; }

private:
	Player* player_ = nullptr;

	std::vector<std::unique_ptr<SampleEnemy>>* enemys_;

};