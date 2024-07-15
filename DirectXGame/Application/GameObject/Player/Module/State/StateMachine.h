#pragma once
#include "Common/IPlayerState.h"
#include <memory>
#include <string>

class Player;

enum class VerticalStates : int32_t {
	kIdle,
	kJump,
	kFall,
};

enum class HorizontalStates : int32_t {
	kIdle,
	kMove,
	kBoost,
};

class StateMachine
{
private:
	// state
	std::unique_ptr<IPlayerState> currentState_;
public: // アクセッサ
	// stateの取得
	IPlayerState* GetState() { return currentState_.get(); }

	std::string name_;

public: // それぞれの名前
	/// <summary>
	/// state変更
	/// </summary>
	/// <param name="type"></param>
	void ChangeRequest(VerticalStates type);
	void ChangeRequest(HorizontalStates type);

public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="player"></param>
	void Initialize(Player* player, std::string name);
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	
private:
	Player* player_ = nullptr;
};
