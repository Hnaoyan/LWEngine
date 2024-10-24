#pragma once
#include "Engine/LwLib/Utillity/FrameTimer.h"
#include "Engine/Math/MathLib.h"
#include "Engine/3D/Instancing/InstancedGroup.h"
#include "Engine/Collision/Collider/ColliderLists.h"
#include "Engine/GlobalVariables/GlobalVariables.h"

#include <memory>

class CollisionManager;
class GPUParticleSystem;
class ParticleEmitter;

namespace BossSystemContext {
	class NormalBullet;
	class TrackingBullet;

	class BulletCluster;
	class MissileCluster;
}

namespace BossSystemContext
{

	enum class TrackType
	{
		kStandard,
		kInferior,
		kSuperior,
	};

	class HealthManager {
	public:
		void Initialize(float maxHealth)
		{
			maxHealth_ = maxHealth;
			currentHealth_ = maxHealth_;
		}
		void TakeDamage(float damage = 1.0f);
		void Heal(float heal = 1.0f);
		bool IsDead() { return isDead_; }

		float GetHPRatio() { return (float)currentHealth_ / (float)maxHealth_; }

	private:
		// マックスHP
		float maxHealth_ = 0;
		// 現在のHP
		float currentHealth_ = 0;
		bool isDead_ = false;
	};

	class ParticleManager {
	public:
		void Initialize(Boss* boss);
		void Update();

	public: // アクセッサ
		void SetGPUParticleSystem(GPUParticleSystem* ptr) { gpuParticle_ = ptr; }
		void OnBulletHit() { isDamage_ = true; }
		bool IsDamage() { return isDamage_; }
	private: // ポインタ関係
		GPUParticleSystem* gpuParticle_ = nullptr;
		Boss* boss_ = nullptr;

		bool isDamage_ = false;

	};
	
	/// <summary>
	/// クラスターを管理するクラス
	/// </summary>
	class BulletManager {
	public:
		/// <summary>
		/// 初期化
		/// </summary>
		/// <param name="models"></param>
		void Initialize(Model* model, Boss* boss);
		/// <summary>
		/// 更新
		/// </summary>
		void Update();
		/// <summary>
		/// 描画
		/// </summary>
		/// <param name="desc"></param>
		void Draw(ModelDrawDesc desc);
		/// <summary>
		/// 衝突更新
		/// </summary>
		/// <param name="manager"></param>
		void CollisionUpdate(CollisionManager* manager);
	public: // USER
		/// <summary>
		/// クラスター作成
		/// </summary>
		void AddCluster();
	public: // アクセッサ
		BulletCluster* GetBeginCluster();
		BulletCluster* GetMissileCluster();
		void SetPlayer(Player* player) { player_ = player; }
		void SetGPUParticle(GPUParticleSystem* ptr) { gpuParticle_ = ptr; }
	private:
		// モデルのリスト
		std::vector<Model*> models_;
		Model* model_ = nullptr;
		// プレイヤーのポインタ
		Player* player_ = nullptr;
		Boss* boss_ = nullptr;
		// 弾のリスト
		std::vector<std::unique_ptr<InstancedGroup>> bulletClusters_;

	private:
		// 
		GPUParticleSystem* gpuParticle_ = nullptr;

	};

	class BulletCluster : public InstancedGroup {
	public: // 仮想関数
		/// <summary>
		/// 初期化
		/// </summary>
		/// <param name="model"></param>
		void Initialize(Model* model) override;
		/// <summary>
		/// 更新
		/// </summary>
		void Update() override;
		/// <summary>
		/// 描画
		/// </summary>
		/// <param name="desc"></param>
		void Draw(ModelDrawDesc desc) override;
		/// <summary>
		/// ImGui
		/// </summary>
		void ImGuiDraw() override;
	public: // USER
		/// <summary>
		/// 衝突処理
		/// </summary>
		/// <param name="manager"></param>
		void CollisionUpdate(CollisionManager* manager);
		/// <summary>
		/// 弾の追加
		/// </summary>
		/// <param name="position"></param>
		/// <param name="direct"></param>
		void AddBullet(const EulerTransform& transform, const Vector3& direct);
		void AddBullet(const EulerTransform& transform, const Vector3& direct, float speed);

		void AddMissile(const EulerTransform& transform, const Vector3& direct, float speed, Player* player);
		void AddMissile(const EulerTransform& transform, const Vector3& direct, float speed, Player* player, TrackType type);
		// GPUParticle
		void SetGPU(GPUParticleSystem* ptr) { gpuParticle_ = ptr; }

		void SetBossPtr(Boss* boss) { boss_ = boss; }
	private:
		uint32_t texture_ = 0;

		GPUParticleSystem* gpuParticle_ = nullptr;
		Boss* boss_ = nullptr;
	};

	class IBullet : public InstancedUnit {
	public: // シリアル番号
		static uint32_t sSerialNumber;
		uint32_t serialNumber_ = 0;
		
		std::string GetTag() { return tag_; }
	protected: // タグ
		std::string tag_;

	public:
		/// <summary>
		/// ImGui描画
		/// </summary>
		/// <param name="name"></param>
		virtual void ImGuiDraw() {};
		/// <summary>
		/// コールバック関数
		/// </summary>
		/// <param name="object"></param>
		virtual void OnCollision(ColliderObject object) = 0;
	public: // アクセッサ
		Sphere* GetCollider() { return &collider_; }
	protected:
		// コライダー
		Sphere collider_;
		// 速度
		Vector3 velocity_ = {};
	};

	class TrackingBullet : public IBullet {
	public:
		/// <summary>
		/// 初期化
		/// </summary>
		/// <param name="model"></param>
		void Initialize() override;
		/// <summary>
		/// 更新
		/// </summary>
		void Update() override;
		/// <summary>
		/// ImGui描画
		/// </summary>
		/// <param name="name"></param>
		void ImGuiDraw() override;
		/// <summary>
		/// コールバック関数
		/// </summary>
		/// <param name="object"></param>
		void OnCollision(ColliderObject object) override;

		void SetBossPtr(Boss* boss) { boss_ = boss; }

	public: // アクセッサ
		EulerTransform GetTransform() { return transform_; }
		Vector3 GetVelocity() { return velocity_; }
		void SetVelocity(const Vector3& velocity) { velocity_ = velocity; }
		void SetPlayer(Player* player) { player_ = player; }
		void SetTrackType(TrackType type) { trackType_ = type; }

	private:
		// 追跡している時間
		FrameTimer trackTimer_;
		// プレイヤーのポインタ
		Player* player_ = nullptr;
		Boss* boss_ = nullptr;

		TrackType trackType_;

		float acceleration_ = 0.2f;

		float lerpRadius_ = 0.0f;

	private:
		// 通常
		void CalcStandardMissile();
		// 劣等
		void CalcInferiorMissile();
		// 優等
		void CalcSuperiorMissile();

	public: // 共通
		static float sTrackingFrame;
		static float sDamping;
		static float sLerpRadius;
		static float sBulletSpeed;
		static float sInitSpeed;

		//static float s

	};

	class NormalBullet : public IBullet {
	public:
		/// <summary>
		/// 初期化
		/// </summary>
		/// <param name="model"></param>
		void Initialize() override;
		/// <summary>
		/// 更新
		/// </summary>
		void Update() override;
		/// <summary>
		/// ImGui描画
		/// </summary>
		/// <param name="name"></param>
		void ImGuiDraw() override;
		/// <summary>
		/// コールバック関数
		/// </summary>
		/// <param name="object"></param>
		void OnCollision(ColliderObject object) override;
	public: // アクセッサ
		EulerTransform GetTransform() { return transform_; }
		void SetVelocity(const Vector3& velocity) { velocity_ = velocity; }

	private:
		Vector3 moveDirect_{};

		float acceleration_ = 0.0f;

	public: // 共通
		static float sAcceleration;

	};


}
