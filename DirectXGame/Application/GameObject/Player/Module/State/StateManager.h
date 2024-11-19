#pragma once
#include "PlayerStates.h"
#include <memory>
#include <optional>

class Player;

class StateManager
{
public:
	enum StateList {
		kIdle, // 待機
		kIdleVertical,	// 垂直の待機
		kIdleHorizontal ,	// 水平の待機
		kMove, // 通常移動
		kJump, // ジャンプ
		kFall, // 落下
		kBoost, // 早い移動
		kQuickBoost, // ダッシュ
		kAssending, // 上昇
	};

public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="player"></param>
	void Initialize(Player* player);
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();
	/// <summary>
	/// 変更リクエスト
	/// </summary>
	/// <param name="newState"></param>
	void ChangeRequest(StateList request);

	IPlayerState* GetState() { return currentState_.get(); }
	PlayerState GetVariant() { return currentState_->GetNowState(); }
private:
	// 自機
	Player* player_ = nullptr;
	// 切り替えリクエスト
	std::optional<StateList> request_;
	// ステートのインスタンス置き場
	std::unique_ptr<IPlayerState> tmpState_;
	std::unique_ptr<IPlayerState> currentState_;
};
