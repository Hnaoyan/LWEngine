//#pragma once
//#include "Engine/3D/Instancing/InstancedGroup.h"
//#include "Engine/GlobalVariables/GlobalVariables.h"
//
//class CollisionManager;
//class GPUParticleSystem;
//class ParticleEmitter;
//class Boss;
//
//namespace BossSystemContext {
//	class NormalBullet;
//	class TrackingBullet;
//
//	class BulletCluster;
//	class MissileCluster;
//}
//
//namespace BossSystemContext
//{
//	class BulletCluster : public InstancedGroup {
//	public: // 仮想関数
//		/// <summary>
//		/// 初期化
//		/// </summary>
//		/// <param name="model"></param>
//		void Initialize(Model* model) override;
//		/// <summary>
//		/// 更新
//		/// </summary>
//		void Update() override;
//		/// <summary>
//		/// 描画
//		/// </summary>
//		/// <param name="desc"></param>
//		void Draw(ModelDrawDesc desc) override;
//		/// <summary>
//		/// ImGui
//		/// </summary>
//		void ImGuiDraw() override;
//	public: // USER
//		/// <summary>
//		/// 衝突処理
//		/// </summary>
//		/// <param name="manager"></param>
//		void CollisionUpdate(CollisionManager* manager);
//		/// <summary>
//		/// 弾の追加
//		/// </summary>
//		/// <param name="position"></param>
//		/// <param name="direct"></param>
//		void AddBullet(const EulerTransform& transform, const Vector3& direct);
//		void AddBullet(const EulerTransform& transform, const Vector3& direct, float speed);
//
//		void AddMissile(const EulerTransform& transform, const Vector3& direct, float speed, Player* player);
//		void AddMissile(const EulerTransform& transform, const Vector3& direct, float speed, Player* player, TrackType type);
//		// GPUParticle
//		void SetGPU(GPUParticleSystem* ptr) { gpuParticle_ = ptr; }
//
//		void SetBossPtr(Boss* boss) { boss_ = boss; }
//	private:
//		GlobalVariables* globalVariables_ = nullptr;
//		GPUParticleSystem* gpuParticle_ = nullptr;
//		Boss* boss_ = nullptr;
//	};
//
//}