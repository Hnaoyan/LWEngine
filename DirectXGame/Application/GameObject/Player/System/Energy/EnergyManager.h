#pragma once
#include "Engine/LwLib/Utillity/FrameTimer.h"

class Player;

namespace PlayerContext
{
	/// <summary>
	/// エネルギーの管理クラス
	/// </summary>
	class EnergyManager
	{
	private:
		// エネルギー関係の値
		struct EnergyData
		{
			// 量
			float maxEnergy = 0.0f;
			float currentEnergy = 0.0f;
			// 減少
			float defaultDecr = 0.0f;
			// 回復量
			float recoveryValue = 12.5f;
			// 回復開始までの時間
			float recoveryFrame = 10.0f;
			// 上昇中か
			bool isAssending = false;
			// リチャージ時間
			FrameTimer rechargeTimer_;
		};

	public:
		// 初期化
		void Initialize(Player* player);
		// 更新
		void Update();
		// エネルギーを瞬時に回復するやつ
		void RecoverGage(float recoverEnergy);
		// エネルギーの割合
		float GetEnergyRatio() { return energy_.currentEnergy / energy_.maxEnergy; }
		float GetNowEnergy() { return energy_.currentEnergy; }
		// ブーストの減少
		void BoostDecrement();
		// 小ダッシュの減少
		void QuickBoostDecre();
		// ダッシュが出来るか
		bool CheckQuickBoost() { return energy_.currentEnergy > (energy_.maxEnergy / 5.0f) ? true : false; }
		// オーバーヒート確認
		bool IsOverheat() { return overheatTimer_.IsActive(); }
		// ImGUi
		void ImGuiDraw();
	private:
		/// <summary>
		/// 減少の処理
		/// </summary>
		/// <param name="value"></param>
		void DecrementProcess(float value);

	private:
		// プレイヤー
		Player* player_ = nullptr;
		// 小ダッシュのリカバリー時間
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
