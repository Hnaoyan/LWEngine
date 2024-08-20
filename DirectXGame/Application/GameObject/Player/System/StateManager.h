#pragma once
#include "../Module/State/PlayerStates.h"
#include <memory>

class Player;

class StateManager
{
public:
	enum StateList {
		kIdle,
		kMove,
		kJump,
		kFall,
		kBoost,
	};
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="player"></param>
	void Initialize(Player* player);
	/// <summary>
	/// 変更リクエスト
	/// </summary>
	/// <param name="newState"></param>
	void ChangeRequest(std::unique_ptr<IPlayerState> newState);
	void ChangeRequest(StateList request);

private:
	// 自機
	Player* player_ = nullptr;

};
