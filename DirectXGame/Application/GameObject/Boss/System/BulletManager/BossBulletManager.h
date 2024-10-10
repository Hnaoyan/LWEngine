#pragma once
#include "Engine/Collision/Collider/ColliderLists.h"
#include "Engine/GlobalVariables/GlobalVariables.h"
#include "Engine/LwLib/Utillity/FrameTimer.h"
#include "Engine/Math/MathLib.h"
#include "Engine/3D/Instancing/InstancedGroup.h"
#include "Application/GameObject/Bullet/User/Factory/BossBulletFactory.h"

#include "../BossSystemStructs.h"

#include <memory>

class CollisionManager;
class GPUParticleSystem;

namespace BossSystemContext
{
	class BulletCluster;

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
	public: // アクッサ
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

		// 弾生成の工場
		std::unique_ptr<BossBulletFactory> factory_;

	private:
		// 
		GPUParticleSystem* gpuParticle_ = nullptr;

	};


}