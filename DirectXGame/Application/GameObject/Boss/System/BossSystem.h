#pragma once
#include "Engine/LwLib/Utillity/FrameTimer.h"
#include "Engine/Math/MathLib.h"

namespace BossSystemContext
{
	class HealthManager {
	public:
		void Initialize(int32_t maxHealth) 
		{
			maxHealth_ = maxHealth;
			currentHealth_ = maxHealth_;
		}
		void TakeDamage(int32_t damage = 1);
		void Heal(int32_t heal = 1);
		bool IsDead() { return isDead_; }
	private:
		// マックスHP
		int32_t maxHealth_;
		// 現在のHP
		int32_t currentHealth_;
		bool isDead_ = false;
	};
}
