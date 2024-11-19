#pragma once
#include "UI/BossUI.h"
#include "Barrier/BossBarrier.h"
#include "BossSystem.h"

class BossFacade
{
public:
	void Initialize(Boss* boss);
	void Update();
	void ImGuiDraw();

	// HPの管理
	BossSystemContext::HealthManager healthManager_;
	BossSystemContext::ParticleManager particleManager_;
	BossSystemContext::BossUIManager uiManager_;
	BossSystemContext::BarrierManager barrierManager_;
};
