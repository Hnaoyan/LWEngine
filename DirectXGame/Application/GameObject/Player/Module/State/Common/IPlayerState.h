#pragma once
#include "Engine/Input/Input.h"
#include "../PlayerStateList.h"

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
	virtual void Initialize();
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
	// ステートマシンのゲッター
	StateMachine* GetStateMachine() { return stateMachine_; }
	// 現在のステートクラス
	PlayerState GetNowState() { return nowState_; }

protected:
	// プレイヤー
	PlayerState nowState_;
	StateMachine* stateMachine_ = nullptr;
	Player* player_ = nullptr;
	Input* input_ = nullptr;

	// 左スティック入力
	Vector2 leftStick_ = {};
	bool isLeftStickActive_ = false;
	bool isBoost_ = false;

};
