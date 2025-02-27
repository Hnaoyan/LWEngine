#pragma once
#include <vector>
#include <memory>
#include "Application/GameObject/Particle/User/ParticlePaths.h"
#include "Engine/3D/Drawer/Triangle3D.h"
#include "../IPlayerSystem.h"

class Player;
class GPUParticleManager;

/// <summary>
/// プレイヤーのパーティクル関係
/// </summary>
namespace PlayerContext {

	class MoveTrail;

	// 移動のパラメータ
	struct MoveParams 
	{
		bool isActive = false;
		uint32_t texture = 0u;
	};

	/// <summary>
	/// マネージャ
	/// </summary>
	class ParticleManager : public ISystem
	{
	public:
		void Initialize(Player* player) override;
		void Update() override;
		void Draw(ICamera* camera);

	private: // 移動
		void MoveParticleUpdate();
		void CreateMoveEmitter();
		MoveParams moveParams_;

	private: // ポインタ関係
		std::unique_ptr<MoveTrail> moveTrail_;
		GPUParticleManager* gpuParticle_ = nullptr;

	public: // アクセッサ
		void SetGPUParticleSystem(GPUParticleManager* ptr) { gpuParticle_ = ptr; }
		bool IsMove() { return moveParams_.isActive; }
		MoveTrail* GetMoveTrail() { return moveTrail_.get(); }
	};

	class MoveTrail
	{
	public:
		// 初期化
		void Initialize(Player* player);
		// 更新
		void Update();
		// 描画
		void Draw(ICamera* camera);

		//void TrailUpdate();

	private:
		// 保存座標と長さ
		std::pair<std::vector<Vector3>, int32_t> trailPoint_;
		// ポリゴン
		std::unique_ptr<Triangle3D> triangle_;

	private:
		WorldTransform worldTransform_;
		Player* player_ = nullptr;
	};

}
