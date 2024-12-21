#pragma once
#include "../ITrackingState.h"
#include "Application/GameObject/Bullet/BulletEnums.h"

#include <memory>

class IBullet;

/// <summary>
/// 弾のステート管理クラス
/// </summary>
class BulletStateMachine
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="bullet"></param>
	BulletStateMachine(IBullet* bullet);
	BulletStateMachine();

	/// <summary>
	/// 更新
	/// </summary>
	void Update(bool isActive);
	// リクエストの作成
	void RequestState(TrackingState state) {
		currentState_->SetRequeset(state);
		nowState_ = state;
	}
public:	// アクセッサ
	ITrackingState* GetCurrentState() { return currentState_.get(); }
	int32_t GetChangeCount() const { return changeCount_; }
	TrackingState GetNowState() const { return nowState_; }
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
	// 現在の状態
	TrackingState nowState_;
	// 弾のポインタ
	IBullet* bullet_ = nullptr;
	// 変更回数
	int32_t changeCount_ = 0;
};
