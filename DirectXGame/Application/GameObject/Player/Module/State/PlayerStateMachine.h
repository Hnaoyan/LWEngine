#pragma once
#include "PlayerStates.h"
#include <memory>
#include <optional>

class Player;

class PlayerStateMachine
{
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
	void ChangeRequest(PlayerStateLists request);

	IPlayerState* GetState() { return currentState_.get(); }
	PlayerState GetVariant() { return currentState_->GetNowState(); }
private:
	// 自機
	Player* player_ = nullptr;
	// 切り替えリクエスト
	std::optional<PlayerStateLists> request_;
	// ステートのインスタンス置き場
	std::unique_ptr<IPlayerState> tmpState_;
	std::unique_ptr<IPlayerState> currentState_;
};
