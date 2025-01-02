#pragma once
#include "Engine/LwLib/LwEnginePaths.h"
#include "../../BulletEnums.h"

#include <optional>

class TrackingBullet;

class StateTransitionManager
{
public:
	StateTransitionManager() {};
	~StateTransitionManager() = default;
	void Initialize(TrackingBullet* bullet);
	void Update();

private:
	// 対象がどっちか
	bool isTargetBoss_ = false;
	bool isCount_ = false;
	// 弾
	TrackingBullet* bullet_ = nullptr;
	// 遷移までの時間
	FrameTimer transitionTimer_;
	// ステートの管理
	std::optional<TrackingState> requestState_ = std::nullopt;

};
