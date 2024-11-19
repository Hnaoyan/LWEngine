#pragma once
#include <optional>
#include "Application/GameObject/Bullet/BulletEnums.h"
#include "Engine/LwLib/LwEnginePaths.h"

class IBullet;
class BulletStateMachine;

/// <summary>
/// 追尾弾の基底ステート
/// </summary>
class ITrackingState
{
public:
	ITrackingState() {};
	virtual ~ITrackingState() = default;

	// 初期化
	virtual void Initialize() {};
	// 開始
	virtual void Enter() {};
	// 更新
	virtual void Update(BulletStateMachine& stateMachine) = 0;
	// 終了
	virtual void Exit() {};

public:
	// リクエストのリセット処理
	void RequestReset() { request_ = std::nullopt; }
	void SetRequeset(TrackingState state) { request_ = state; }

public:
	void SetBullet(IBullet* bullet) { bullet_ = bullet; }
	std::optional<TrackingState> GetChangeRequest() { return request_; }
protected:
	IBullet* bullet_ = nullptr;
	// ステート変更先＋リクエストフラグ
	std::optional<TrackingState> request_ = std::nullopt;

	// 変更タイマー
	FrameTimer timer_;
};
