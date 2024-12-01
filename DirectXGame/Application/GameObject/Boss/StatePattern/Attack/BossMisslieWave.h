#pragma once
#include "../StateMachine.h"
#include "Engine/LwLib/LwEnginePaths.h"

namespace BossState
{
	// 波状でミサイルを撃ってくるステート（連続して撃つやつ
	class MissileWaveState : public IState
	{
	private:
		// ミサイルのフェーズ管理
		enum MissilePhase
		{
			kPhase1,
			kPhase2,
			kPhase3,
			kSize,
		};
	public:
		void Initialize() override;
		void Update() override;
		void Exit() override;

	private:
		void SpawnMissile();
		void GenerateMissile(const Vector3& direct, TrackingAttribute type);

		// 生成間隔
		FrameTimer spawnTimer_;

	};
}
