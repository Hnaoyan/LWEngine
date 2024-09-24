#pragma once
#include <stdint.h>
#include "Engine/LwLib/Utillity/FrameTimer.h"

class Player;

namespace PlayerContext
{
	class HealthManager
	{
	public:
		/// <summary>
		/// 初期化
		/// </summary>
		/// <param name="player"></param>
		/// <param name="maxHP"></param>
		void Initialize(Player* player, uint32_t maxHP);
		/// <summary>
		/// 更新処理
		/// </summary>
		void Update();
		/// <summary>
		/// ダメージ処理
		/// </summary>
		/// <param name="damage"></param>
		void TakeDamage(uint32_t damage = 1);

		float GetHPRatio() { return (float)hitPoint_.currentHealth / (float)hitPoint_.maxHealth; }
	private:

		struct HealthData {
			uint32_t maxHealth = 0;
			uint32_t currentHealth = 0;
			FrameTimer invincibility;
			FrameTimer damageEffectDuration;
		};
		// HP関係
		HealthData hitPoint_;

		Player* player_ = nullptr;

	};

}
