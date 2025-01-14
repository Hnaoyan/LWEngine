#pragma once
#include "Engine/LwLib/LwEnginePaths.h"
#include "../../BulletEnums.h"

#include <vector>
#include <optional>
#include <unordered_map>

class TrackingBullet;

class StateTransitionManager
{
public:
	StateTransitionManager() {};
	~StateTransitionManager() = default;
	void Initialize(TrackingBullet* bullet);
	void Update();

	void DefaultTransition();
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

	std::unordered_map<std::string, std::vector<TrackingState>> nameTables_;
	int32_t currentTable_ = 0;
};
