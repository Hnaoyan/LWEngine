#pragma once
#include "Engine/LwLib/LwEnginePaths.h"
#include "../../BulletEnums.h"

#include <vector>
#include <optional>
#include <unordered_map>

class TrackingBullet;

/// <summary>
/// ステートの遷移管理クラス
/// </summary>
class StateTransitionManager
{
public:
	StateTransitionManager() {};
	~StateTransitionManager() = default;
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="bullet"></param>
	void Initialize(TrackingBullet* bullet);
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 通常の遷移
	/// </summary>
	void DefaultTransition();
	/// <summary>
	/// 決めた遷移
	/// </summary>
	void SetUpTransition();

private:
	// 対象がどっちか
	bool isTargetBoss_ = false;
	bool isCount_ = false;
	bool isTransition_ = false;
	// 弾
	TrackingBullet* bullet_ = nullptr;
	// 遷移までの時間
	FrameTimer transitionTimer_;
	// ステートの管理
	std::optional<TrackingState> requestState_ = std::nullopt;
	// テーブル
	std::unordered_map<std::string, std::vector<TrackingState>> nameTables_;
	int32_t currentTable_ = 0;
};
