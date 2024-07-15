#pragma once
#include "Common/IPlayerState.h"
#include "StateEnum.h"
#include "../../Player.h"

#include <cassert>
#include <memory>
#include <string>
#include <optional>

class Player;

template<typename T>
class StateMachine
{
private:
	// state
	std::unique_ptr<IPlayerState> currentState_;
public: // アクセッサ
	// stateの取得
	IPlayerState* GetState() {
		return currentState_.get();
	}

	~StateMachine() = default;

	std::string name_;

public: // それぞれの名前
	/// <summary>
	/// state変更
	/// </summary>
	/// <param name="type"></param>
	void ChangeRequest(VerticalStates type)
	{
		std::unique_ptr<IPlayerState> newState;
		switch (type)
		{
			// 縦方向
		case VerticalStates::kIdle:
			newState = std::make_unique<IdleState>();
			break;
		case VerticalStates::kJump:
			newState = std::make_unique<JumpingState>();
			break;
		case VerticalStates::kFall:
			newState = std::make_unique<FallingState>();
			break;
		}
		if (currentState_) {
			currentState_->Exit();
		}
		// 新しいステートの初期化
		newState->PreInitialize(player_);
		newState->Initialize();
		currentState_ = std::move(newState);
	}
	void ChangeRequest(HorizontalStates type)
	{
		std::unique_ptr<IPlayerState> newState;
		switch (type)
		{
		// 水平方向
		case HorizontalStates::kIdle:
			newState = std::make_unique<IdleState>();
			break;
		case HorizontalStates::kMove:
			newState = std::make_unique<MovingState>();
			break;
		case HorizontalStates::kBoost:
			newState = std::make_unique<BoostState>();
			break;
		}
		if (currentState_) {
			currentState_->Exit();
		}
		// 新しいステートの初期化
		newState->PreInitialize(player_);
		newState->Initialize();
		currentState_ = std::move(newState);
	}
	//void ChangeRequest(HorizontalStates type);

public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="player"></param>
	void Initialize(Player* player, std::string name){
		assert(player);
		player_ = player;

		name_ = name;

		if (currentState_ == nullptr) {
			// 最初の初期化
			currentState_ = std::make_unique<IdleState>();
			currentState_->PreInitialize(player_);
			currentState_->Initialize();
		}

	}

	/// <summary>
	/// 更新
	/// </summary>
	void Update(){
		// 実体があるか
		if (currentState_) {
			// 更新フラグ
			currentState_->InputHandle();
			currentState_->Update();
		}
	}

	
private:
	Player* player_ = nullptr;
	//T stateType_;
	//std::optional<T> request_ = std::nullopt;
};
