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
	struct LockOnData {
		float threshold = 0.0f;
		float minDistanceZ = 5.0f;
		float maxDistanceZ = 50.0f;
	};
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="player"></param>
	void Initialize(Player* player);
	/// <summary>
	/// 入力による対象設定・対象解除
	/// </summary>
	void ToggleLockOn(ICamera* camera);
	/// <summary>
	/// ImGui
	/// </summary>
	void ImGuiDraw();
private:
	/// <summary>
	/// ターゲット解除
	/// </summary>
	void TargetRelease();
	/// <summary>
	/// 敵を探す処理
	/// </summary>
	/// <param name="camera"></param>
	void SearchTarget(ICamera* camera);

public:
	// リストの設定
	void SetEnemyList(std::vector<std::unique_ptr<SampleEnemy>>* lists) { enemys_ = lists; }
	// ターゲットのポインタ
	SampleEnemy* GetTarget() { return target_; }
	// 対象があるかどうか
	bool ExistTarget() const { return target_ ? true : false; }

private:
	// プレイヤー
	Player* player_ = nullptr;
	// リスト
	std::vector<std::unique_ptr<SampleEnemy>>* enemys_;
	// ターゲット
	SampleEnemy* target_ = nullptr;
	// 内積の閾値
	LockOnData data{};
};