#include "BossFacade.h"

void BossFacade::Initialize(Boss* boss)
{
	healthManager_.Initialize(20);

	particleManager_.Initialize(boss);

	uiManager_.Initialize(boss);
}

void BossFacade::Update()
{
	particleManager_.Update();
}
