#include "BossSystem.h"

void BossSystemContext::HealthManager::TakeDamage(int32_t damage)
{
	currentHealth_ -= damage;
	// 死亡処理
	if (currentHealth_ < 0) {
		isDead_ = true;
		currentHealth_ = 0;
	}
}

void BossSystemContext::HealthManager::Heal(int32_t heal)
{
	if (currentHealth_ < maxHealth_) {
		currentHealth_ += heal;
	}
}
