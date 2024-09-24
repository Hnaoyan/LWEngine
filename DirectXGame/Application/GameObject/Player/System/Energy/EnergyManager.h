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
			// 量
			float maxEnergy = 0.0f;
			float currentEnergy = 0.0f;
			// 減少
			float defaultDecr = 0.0f;
			// 
			bool isAssending = false;
			// リチャージ時間
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
		EnergyData energy_{};
	};

}
