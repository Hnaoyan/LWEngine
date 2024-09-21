#include "BossFacade.h"

void BossFacade::Initialize(Boss* boss)
{
	// HP
	healthManager_.Initialize(20);
	// パーティクル
	particleManager_.Initialize(boss);
	// UI
	uiManager_.Initialize(boss);
	// Barrier
	barrierManager_.Initialize(boss);
}

void BossFacade::Update()
{
	// パーティクル更新
	particleManager_.Update();
	// バリア更新
	barrierManager_.Update();
}
