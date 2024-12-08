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
		void GenerateMissile(const Vector3& direct, TrackingAttribute type);
	private:
		// 発射間隔
		FrameTimer attackTimer_;

	};

}

