#pragma once
#include "IPlayerState.h"
#include "../StateMachine.h"
#include <array>

class IdleState : public IPlayerState
{
public: // 継承部
	void Initialize() override;
	void Update() override;
	void Exit() override;
	void InputHandle() override;

private:
	void VerticalUpdate();
	void HorizontalUpdate();

	// 関数ポインタの構造体
	struct UpdateFunc {
		void(IdleState::* updateFunc_)() = nullptr;
	};

	inline static const std::array<UpdateFunc, static_cast<uint32_t>(StateMachine::StateType::kNone)> updateFunc{
		UpdateFunc{&IdleState::VerticalUpdate,},
		UpdateFunc{&IdleState::HorizontalUpdate,}
	};
};
