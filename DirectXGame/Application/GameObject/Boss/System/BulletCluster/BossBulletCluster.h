#pragma once
#include "Engine/Collision/Collider/ColliderLists.h"
#include "Engine/GlobalVariables/GlobalVariables.h"
#include "Engine/LwLib/Utillity/FrameTimer.h"
#include "Engine/Math/MathLib.h"
#include "Engine/3D/Instancing/InstancedGroup.h"

#include "../BossSystemStructs.h"

#include <memory>


class CollisionManager;
class GPUParticleSystem;
class ParticleEmitter;

namespace BossSystemContext {
	class NormalBullet;
	class TrackingBullet;
}

namespace BossSystemContext
{
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
		// 通常弾の追加
		void AddBullet(const EulerTransform& transform, const Vector3& direct);
		void AddBullet(const EulerTransform& transform, const Vector3& direct, float speed);
		// 誘導弾の追加
		void AddMissile(const EulerTransform& transform, const Vector3& direct, float speed, Player* player);
		void AddMissile(const EulerTransform& transform, const Vector3& direct, float speed, Player* player, TrackType type);

	public: // アクセッサ
		void SetBossPtr(Boss* boss) { boss_ = boss; }
	private:
		uint32_t texture_ = 0;
		// 弾の名前
		std::string parentTag_;
		GlobalVariables* globalVariables_ = nullptr;
		Boss* boss_ = nullptr;
	};

}
