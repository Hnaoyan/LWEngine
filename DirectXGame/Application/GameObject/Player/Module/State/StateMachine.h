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
public: // どちらかを判断
	enum class StateType : int32_t {
		kVertical,
		kHorizontal,
		kNone,
	};

private:
	// state
	std::unique_ptr<IPlayerState> currentState_;

	StateType stateType_ = StateType::kNone;

public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="player"></param>
	void Initialize(Player* player, StateType type);
	/// <summary>
	/// 更新
	/// </summary>
	void Update();

public: // それぞれの名前
	/// <summary>
	/// state変更
	/// </summary>
	/// <param name="type"></param>
	void ChangeRequest(VerticalStates type);
	void ChangeRequest(HorizontalStates type);

public: // アクセッサ
	// stateの取得
	IPlayerState* GetState() { return currentState_.get(); }
	StateType GetStateType() const { return stateType_; }

private:
	Player* player_ = nullptr;
	std::string name_;

};
