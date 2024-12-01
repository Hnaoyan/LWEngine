#pragma once
#include "../StateMachine.h"
#include "Engine/LwLib/LwEnginePaths.h"

namespace BossState
{
	// 波状でミサイルを撃ってくるステート（連続して撃つやつ
	class MissileContainerState : public IState
	{
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
