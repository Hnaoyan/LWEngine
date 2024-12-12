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

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize() {};
	/// <summary>
	/// 開始
	/// </summary>
	virtual void Enter() {};
	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="stateMachine"></param>
	virtual void Update(BulletStateMachine& stateMachine) = 0;
	/// <summary>
	/// 終了
	/// </summary>
	virtual void Exit() {};

public:
	// リクエストのリセット処理
	void RequestReset() { request_ = std::nullopt; }
	void SetRequeset(TrackingState state) { request_ = state; }

public:
	void SetBullet(IBullet* bullet) { bullet_ = bullet; }
	std::optional<TrackingState> GetChangeRequest() { return request_; }
protected:
	// 弾のポインタ
	IBullet* bullet_ = nullptr;
	// ステート変更先＋リクエストフラグ
	std::optional<TrackingState> request_ = std::nullopt;
	// 変更用のタイマー
	FrameTimer timer_;
};
