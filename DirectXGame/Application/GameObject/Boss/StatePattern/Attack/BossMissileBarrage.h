#pragma once
#include "../StateMachine.h"
#include "Engine/LwLib/LwEnginePaths.h"

namespace BossState
{
	class MissileBarrageState : public IState
	{
	public:
		void Initialize() override;
		void Update() override;
		void Exit() override;
	private:
		void Attack();
	private:
		// 発射間隔
		FrameTimer attackTimer_;
		int32_t fireCount_ = 0;
	};

}

