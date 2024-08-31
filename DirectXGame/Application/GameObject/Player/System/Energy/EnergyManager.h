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
		float GetEnergyRatio() { return energy_.currentEnergy / energy_.maxEnergy; }
		float GetNowEnergy() { return energy_.currentEnergy; }
		void QuickBoostDecre();
		// ダッシュが出来るか
		bool CheckQuickBoost() { return energy_.currentEnergy > (energy_.maxEnergy / 5.0f) ? true : false; }
		//void JumpDecre();

	private:
		Player* player_ = nullptr;
		FrameTimer quickBoostRecoveryTime_;
	public:
		// エネルギー情報
		EnergyData energy_;
	};

}
