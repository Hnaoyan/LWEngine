#include "BossSystem.h"
#include "Engine/Collision/CollisionManager.h"
#include "Engine/Particle/GPUParticleSystem.h"
#include "Engine/3D/ModelUtility/ModelManager.h"
#include "Engine/2D/TextureManager.h"
#include "Application/GameObject/Particle/User/ParticlePaths.h"
#include "../Boss.h"

void BossSystemContext::HealthManager::TakeDamage(float damage)
{
	// 減少処理
	currentHealth_ -= damage;
	// 死亡処理
	if (currentHealth_ <= 0) {
		isDead_ = true;
		currentHealth_ = 0;
	}
}

void BossSystemContext::HealthManager::Heal(float heal)
{
	// 回復処理
	if (currentHealth_ < maxHealth_) {
		currentHealth_ += heal;
	}
}

void BossSystemContext::ParticleManager::Initialize(Boss* boss)
{
	// ポインタの設定
	boss_ = boss;
	SetGPUParticleSystem(boss_->GetGPUParticle());
	// ダメージのエミッタ作成
	CreateDamageEmitter();
	// バリアわれるときのエミッタ作成
	CreateBarrierBreakEmitter();

}
void BossSystemContext::ParticleManager::Update()
{
	damage_.activeTimer.Update();
	if (damage_.activeTimer.IsEnd()) {
		damage_.isActive = false;
	}

	barrierBreak_.activeTimer.Update();
	if (barrierBreak_.activeTimer.IsEnd()) {
		barrierBreak_.isActive = false;
	}

}

void BossSystemContext::ParticleManager::CreateDamageEmitter()
{

	damage_.activeFrame = 3.0f;
	damage_.isActive = false;

	std::unique_ptr<GPUParticleEmitter> instance = std::make_unique<BossParticle::DamageEffect>();
	instance->Initialize(ModelManager::GetModel("Plane"));
	BossParticle::DamageEffect* damage = static_cast<BossParticle::DamageEffect*>(instance.get());
	damage->SetTextureHandle(TextureManager::Load("Resources/Effect/effect.png"));
	damage->SetBoss(boss_);
	gpuParticle_->CreateEmitter(std::move(instance), "BossDamage");
}

void BossSystemContext::ParticleManager::CreateBarrierBreakEmitter()
{
	barrierBreak_.activeFrame = 3.0f;
	barrierBreak_.isActive = false;

	std::unique_ptr<GPUParticleEmitter> instance = std::make_unique<BossParticle::BarrierBreakEffect>();
	instance->Initialize(ModelManager::GetModel("Plane"));
	BossParticle::BarrierBreakEffect* barrierBreak = static_cast<BossParticle::BarrierBreakEffect*>(instance.get());
	barrierBreak->SetTextureHandle(TextureManager::Load("Resources/Effect/effect.png"));
	barrierBreak->SetBoss(boss_);
	gpuParticle_->CreateEmitter(std::move(instance), "BossBarrierBreak");
}
