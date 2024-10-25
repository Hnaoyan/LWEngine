#pragma once
#include "../ITrackingState.h"
#include "Application/GameObject/Bullet/BulletEnums.h"

#include <memory>

class IBullet;

class BulletStateMachine
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="bullet"></param>
	BulletStateMachine(IBullet* bullet) { bullet_ = bullet; }
	/// <summary>
	/// 変更リクエスト
	/// </summary>
	/// <param name="state"></param>
	void ChangeRequest(std::unique_ptr<ITrackingState> state);
	void ChangeRequest(TrackingState state);

	std::unique_ptr<ITrackingState> BuildState(TrackingState newState);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

private:
	// ステート
	std::unique_ptr<ITrackingState> currentState_;
	// 弾のポインタ
	IBullet* bullet_ = nullptr;
};
