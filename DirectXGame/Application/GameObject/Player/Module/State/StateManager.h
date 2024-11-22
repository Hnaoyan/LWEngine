#pragma once
#include "PlayerStates.h"
#include "PlayerStateMachine.h"
#include <memory>
#include <optional>

class Player;

class PlayerStateManager
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

public:
	PlayerStateMachine* GetHorizontal() { return horizontalState_.get(); }
	PlayerStateMachine* GetVertical() { return verticalState_.get(); }
private:
	// 自機
	Player* player_ = nullptr;

	std::unique_ptr<PlayerStateMachine> horizontalState_;
	std::unique_ptr<PlayerStateMachine> verticalState_;

};
