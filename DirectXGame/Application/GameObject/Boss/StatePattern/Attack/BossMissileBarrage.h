#pragma once
#include "../StateMachine.h"
#include "Engine/LwLib/Utillity/FrameTimer.h"

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
		void GenerateMissile(const Vector3& direct, TrackingType type);
	private:
		// 発射間隔
		FrameTimer attackTimer_;

	};

}

