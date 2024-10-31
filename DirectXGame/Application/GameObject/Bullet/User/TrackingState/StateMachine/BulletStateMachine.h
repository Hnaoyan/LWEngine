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
	BulletStateMachine() {};

	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	
	// リクエストの作成
	void RequestState(TrackingState state) { currentState_->SetRequeset(state); }
	// 現在のステートのゲッター
	ITrackingState* GetCurrentState() { return currentState_.get(); }
private: // メンバ関数
	/// <summary>
	/// ステートの作成
	/// </summary>
	/// <param name="newState"></param>
	/// <returns></returns>
	std::unique_ptr<ITrackingState> BuildState(TrackingState newState);
	/// <summary>
	/// 変更リクエストの処理
	/// </summary>
	/// <param name="state"></param>
	void ChangeRequest(std::unique_ptr<ITrackingState> state);
	void ChangeRequest(TrackingState state);

private:
	// ステート
	std::unique_ptr<ITrackingState> currentState_;
	// 弾のポインタ
	IBullet* bullet_ = nullptr;
};
