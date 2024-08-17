#pragma once
#include "Engine/LwLib/Utillity/FrameTimer.h"
#include "Engine/Math/MathLib.h"
#include "Engine/3D/Instancing/InstancedGroup.h"
#include "Engine/Collision/Collider/ColliderLists.h"

class CollisionManager;
namespace BossSystemContext {
	class NormalBullet;
	class TrackingBullet;

	class BulletCluster;
	class MissileCluster;
}

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
	
	/// <summary>
	/// クラスターを管理するクラス
	/// </summary>
	class BulletManager {
	public:
		/// <summary>
		/// 初期化
		/// </summary>
		/// <param name="models"></param>
		void Initialize(Model* model);
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
		void SetPlayer(Player* player) {
			player_ = player;
		}

	private:
		// モデルのリスト
		std::vector<Model*> models_;
		Model* model_ = nullptr;
		// プレイヤーのポインタ
		Player* player_ = nullptr;
		// 弾のリスト
		std::vector<std::unique_ptr<InstancedGroup>> bulletClusters_;
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

	private:
		uint32_t texture_ = 0;
	};

	class TrackingBullet : public InstancedUnit {
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
		void ImGuiDraw();
		/// <summary>
		/// コールバック関数
		/// </summary>
		/// <param name="object"></param>
		void OnCollision(ColliderObject object);
	public: // アクセッサ
		Sphere* GetCollider() { return &collider_; }
		EulerTransform GetTransform() { return transform_; }
		void SetVelocity(const Vector3& velocity) { velocity_ = velocity; }
		void SetPlayer(Player* player) { player_ = player; }
	private:
		// コライダー
		Sphere collider_;
		// 速度
		Vector3 velocity_ = {};

		// 追跡している時間
		FrameTimer trackTimer_;
		// プレイヤーのポインタ
		Player* player_ = nullptr;
	};

	class NormalBullet : public InstancedUnit {
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
		void ImGuiDraw();
		/// <summary>
		/// コールバック関数
		/// </summary>
		/// <param name="object"></param>
		void OnCollision(ColliderObject object);
	public: // アクセッサ
		Sphere* GetCollider() { return &collider_; }
		EulerTransform GetTransform() { return transform_; }
		void SetVelocity(const Vector3& velocity) { velocity_ = velocity; }
	private:
		// コライダー
		Sphere collider_;
		// 速度
		Vector3 velocity_ = {};
	};


}
