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
			// 回復量
			float recoveryValue = 10.0f;
			// 回復開始までの時間
			float recoveryFrame = 10.0f;
			// 上昇中か
			bool isAssending = false;
			// リチャージ時間
			FrameTimer rechargeTimer_;
		};

	public:
		void Initialize(Player* player);
		void Update();
		// エネルギーの割合
		float GetEnergyRatio() { return energy_.currentEnergy / energy_.maxEnergy; }
		float GetNowEnergy() { return energy_.currentEnergy; }
		void BoostDecrement();
		void QuickBoostDecre();
		// ダッシュが出来るか
		bool CheckQuickBoost() { return energy_.currentEnergy > (energy_.maxEnergy / 5.0f) ? true : false; }
		bool IsOverheat() { return overheatTimer_.IsActive(); }
		// ImGUi
		void ImGuiDraw();
	private:
		void DecrementProcess(float value);

	private:
		Player* player_ = nullptr;
		FrameTimer quickBoostRecoveryCoolTime_;

		// オーバーヒートの時間
		FrameTimer overheatTimer_;
		// ダッシュの消費量
		float decrementRate_ = 3.0f;
		// 小ダッシュの消費の減少率
		float quickRate_ = 0.75f;

	public:
		// エネルギー情報
		EnergyData energy_{};
	};

}
