#pragma once
#include "../StateMachine.h"
#include "Engine/LwLib/LwEnginePaths.h"

namespace BossState
{

	class TurnMissileState : public IState
	{
	public:
		void Initialize() override;
		void Update() override;
		void Exit() override;


	};

}
