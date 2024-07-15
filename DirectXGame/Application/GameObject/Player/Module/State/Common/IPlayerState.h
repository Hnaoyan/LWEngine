#pragma once
#include "Engine/Input/Input.h"
//#include "../StateMachine.h"

class Player;
class StateManager;

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
	void PreInitialize(Player* player);
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
protected:
	// プレイヤー
	Player* player_ = nullptr;
	StateManager* stateManager_ = nullptr;
	Input* input_ = nullptr;

	// 左スティック入力
	Vector2 leftStick_ = {};
	bool isLeftStickActive_ = false;

};
