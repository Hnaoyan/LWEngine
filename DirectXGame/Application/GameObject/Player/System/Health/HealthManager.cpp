#include "HealthManager.h"
#include "../../Player.h"
#include "Application/GameSystem/GameSystem.h"
#include "Engine/PostEffect/PostEffectRender.h"

void PlayerContext::HealthManager::Initialize(Player* player, uint32_t maxHP)
{
	player_ = player;
	hitPoint_.maxHealth = maxHP;
	hitPoint_.currentHealth = maxHP;
}

void PlayerContext::HealthManager::Update()
{
	// クールタイムの処理
	hitPoint_.invincibility.Update();
	hitPoint_.damageEffectDuration.Update(GameSystem::GameSpeedFactor());

	if (hitPoint_.invincibility.IsActive()) {
		PostEffectRender::sPostEffect = Pipeline::PostEffectType::kVignette;
	}
	if (hitPoint_.invincibility.IsEnd()) {
		PostEffectRender::sPostEffect = Pipeline::PostEffectType::kNormal;
	}
}

void PlayerContext::HealthManager::TakeDamage(uint32_t damage)
{
	// 無敵時間
	if (hitPoint_.invincibility.IsActive()) {
		return;
	}

	hitPoint_.currentHealth -= damage;

	hitPoint_.invincibility.Start(30.0f);
	hitPoint_.damageEffectDuration.Start(5.0f);

	if (hitPoint_.currentHealth < 1) {
		player_->SetIsDead(true);
		hitPoint_.currentHealth = 0;
	}
	else {
		player_->camera_->ExecuteShake(15.0f, 2.0f);
	}

}