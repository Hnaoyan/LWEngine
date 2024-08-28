#pragma once
#include "Engine/LwLib/Utillity/FrameTimer.h"

class Player;

namespace PlayerContext
{
	class EnergyManager
	{
	private:
		struct EnergyData
		{
			float maxEnergy;
			float currentEnergy;

			float defaultDecr;
			bool isAssending;

			FrameTimer rechargeTimer_;
		};

	public:
		void Initialize(Player* player);
		void Update();
		//void DashInit();
		//void JumpInit();

	private:
		Player* player_ = nullptr;

		// エネルギー情報
		EnergyData energy_;
	};

}
