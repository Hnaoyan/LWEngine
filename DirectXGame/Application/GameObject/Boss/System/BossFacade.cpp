#include "BossFacade.h"
#include "imgui.h"

void BossFacade::Initialize(Boss* boss)
{
	// HP
	healthManager_.Initialize(60);
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

void BossFacade::ImGuiDraw()
{
	if (ImGui::BeginTabBar("Facade"))
	{
		if (ImGui::BeginTabItem("Barrier")) {
			barrierManager_.ImGuiDraw();
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}
}
