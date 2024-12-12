#pragma once
#include "Engine/Particle/Emitter/GPUParticleEmitter.h"
#include "Engine/3D/Instancing/InstancedUnit.h"
#include "../../EmitterTypes.h"

class Player;

namespace PlayerParticle
{
	/// <summary>
	/// プレイヤー用のコンポーネント
	/// </summary>
	class EmitterComponent
	{
	public:
		void SetPlayer(Player* player) { player_ = player; }
	protected:
		Player* player_ = nullptr;
	};
	/// <summary>
	/// 移動エフェクト
	/// </summary>
	class MoveEffect : public GPUParticleEmitter, public EmitterComponent
	{
	public:
		virtual void Initialize(Model* model, uint32_t textureHandle = 0);
		virtual void Update();
		virtual void Draw(ICamera* camera);

	private:

	};
	/// <summary>
	/// 弾のエフェクト
	/// </summary>
	class BulletEffect : public GPUParticleEmitter, public EmitterComponent
	{
	public:
		virtual void Initialize(Model* model, uint32_t textureHandle = 0);
		virtual void Update();
		virtual void Draw(ICamera* camera);

	private:
		// 
		InstancedUnit* instance_ = nullptr;
	};

}
