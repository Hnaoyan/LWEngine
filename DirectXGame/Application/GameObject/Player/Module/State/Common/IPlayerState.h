#pragma once
#include "Engine/Input/Input.h"

class Player;
class StateManager;
class StateMachine;

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
	void PreInitialize(Player* player, StateMachine* stateMachine);
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
public: // アクセッサ
	StateMachine* GetStateMachine() { return stateMachine_; }

protected:
	// プレイヤー
	Player* player_ = nullptr;
	StateManager* stateManager_ = nullptr;
	StateMachine* stateMachine_ = nullptr;
	Input* input_ = nullptr;

	// 左スティック入力
	Vector2 leftStick_ = {};
	bool isLeftStickActive_ = false;
	bool isBoost_ = false;

};
