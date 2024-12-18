#pragma once
#include "Engine/Collision/Collider/ColliderLists.h"
#include "Engine/GlobalVariables/GlobalVariables.h"
#include "Engine/LwLib/Utillity/FrameTimer.h"
#include "Engine/Math/MathLib.h"
#include "Engine/3D/Instancing/InstancedGroup.h"

#include "BossSystemStructs.h"

#include <memory>

class CollisionManager;
class GPUParticleManager;
class GPUParticleEmitter;

namespace BossSystemContext
{
	/// <summary>
	/// 体力
	/// </summary>
	class HealthManager {
	public:
		void Initialize(float maxHealth)
		{
			maxHealth_ = maxHealth;
			currentHealth_ = maxHealth_;
		}
		// ダメージ
		void TakeDamage(float damage = 1.0f);
		// 回復
		void Heal(float heal = 1.0f);
		// 死亡
		bool IsDead() { return isDead_; }
		// HPレート
		float GetHPRatio() { return (float)currentHealth_ / (float)maxHealth_; }
	private:
		// マックスHP
		float maxHealth_ = 0;
		// 現在のHP
		float currentHealth_ = 0;
		bool isDead_ = false;
	};
	/// <summary>
	/// パーティクル管理
	/// </summary>
	class ParticleManager {
	public:
		// 初期化
		void Initialize(Boss* boss);
		// 更新
		void Update();
		// オンヒット
		void OnBulletHit() {
			damage_.isActive = true;
			damage_.activeTimer.Start(damage_.activeFrame);
		}
		void BarrierBreakExcept() {
			barrierBreak_.isActive = true;
			barrierBreak_.activeTimer.Start(barrierBreak_.activeFrame);
		}

	public: // アクセッサ
		void SetGPUParticleSystem(GPUParticleManager* ptr) { gpuParticle_ = ptr; }
		bool IsDamage() { return damage_.isActive; }
		bool IsBarrierBreak() { return barrierBreak_.isActive; }
	private: // ダメージ
		// 生成
		void CreateDamageEmitter();
		void CreateBarrierBreakEmitter();
		// ダメージ関係のパラメータ
		EffectParams damage_;
		// バリア割れるエフェクト
		EffectParams barrierBreak_{};
	private: // ポインタ関係
		GPUParticleManager* gpuParticle_ = nullptr;
		Boss* boss_ = nullptr;

	};
	


}
