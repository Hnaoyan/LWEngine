#pragma once
#include "Engine/Input/Input.h"
#include "PlayerStateList.h"

class Player;
class PlayerStateMachine;

class IPlayerState
{
public:
	IPlayerState() {};
	virtual ~IPlayerState() = default;

public:
	/// <summary>
	/// 前初期化
	/// </summary>
	/// <param name="player"></param>
	void PreInitialize(Player* player, PlayerStateMachine* stateMachine);
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="player"></param>
	virtual void Initialize() = 0;
	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() = 0;
	/// <summary>
	/// 終了時
	/// </summary>
	virtual void Exit() = 0;
	/// <summary>
	/// 入力による処理
	/// </summary>
	virtual void InputHandle();

	/// <summary>
	/// 今のステート
	/// </summary>
	/// <returns></returns>
	PlayerState GetNowState() {
		return nowState_;
	}

protected:

	PlayerState nowState_;

protected:
	// プレイヤー
	Player* player_ = nullptr;
	PlayerStateMachine* stateMachine_ = nullptr;
	Input* input_ = nullptr;

	// 左スティック入力
	Vector2 leftStick_ = {};
	bool isLeftStickActive_ = false;

};
