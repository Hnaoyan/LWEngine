#pragma once
#include "Engine/Input/Input.h"

class Player;

class IPlayerState
{
public:
	/// <summary>
	/// 前初期化
	/// </summary>
	/// <param name="player"></param>
	void PreInitialize(Player* player) {
		player_ = player;
		input_ = Input::GetInstance();
	}
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
	virtual void InputHandle() = 0;
protected:
	// プレイヤー
	Player* player_ = nullptr;
	Input* input_ = nullptr;
};
