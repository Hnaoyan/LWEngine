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
	invisibleEffect_.Update();

	// それぞれの色の変わり
	if (invisibleEffect_.activeTimer.IsActive()) {
		// アルファの代わり
		if (invisibleEffect_.upTimer.IsActive()) {
			invisibleEffect_.color.w = Ease::Easing(0.0f, 1.0f, invisibleEffect_.upTimer.GetElapsedFrame());
		}
		else if (invisibleEffect_.downTimer.IsActive()) {
			invisibleEffect_.color.w = Ease::Easing(1.0f, 0.0f, invisibleEffect_.upTimer.GetElapsedFrame());
		}
		// 切り替わり
		if (invisibleEffect_.downTimer.IsEnd()) {
			invisibleEffect_.upTimer.Start(invisibleEffect_.returnFrame);
		}
		else if (invisibleEffect_.upTimer.IsEnd()) {
			invisibleEffect_.downTimer.Start(invisibleEffect_.returnFrame);
		}
	}
	else if (invisibleEffect_.activeTimer.IsEnd()) {
		invisibleEffect_.color.w = 1.0f;
	}

	hitPoint_.damageEffectDuration.Update(GameSystem::GameSpeedFactor());
	if (invisibleEffect_.activeTimer.IsEnd()) {
		PostEffectManager::sDamageEffect.Finalize();
	}

	if (hitPoint_.currentHealth <= 0) {
		player_->SetIsDead(true);
	}

}

void PlayerContext::HealthManager::TakeDamage(uint32_t damage)
{
	// 無敵時間
	if (invisibleEffect_.activeTimer.IsActive()) {
		return;
	}

	hitPoint_.currentHealth -= damage;
	invisibleEffect_.upTimer.Start(invisibleEffect_.returnFrame);
	invisibleEffect_.activeTimer.Start(30.0f); // 無敵時間
	hitPoint_.damageEffectDuration.Start(5.0f); // エフェクトの時間
	PostEffectManager::sDamageEffect.Initialize();	// ダメージエフェクト

	if (hitPoint_.currentHealth < 1) {
		player_->SetIsDead(true);
		hitPoint_.currentHealth = 0;
	}
	else {
		player_->camera_->ExecuteShake(15.0f, 2.0f);
	}

}