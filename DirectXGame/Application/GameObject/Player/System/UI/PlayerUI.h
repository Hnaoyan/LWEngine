#pragma once
#include "Application/GameSystem/UI/UIStructs.h"
#include "Engine/GlobalVariables/GlobalVariables.h"

class Player;
class Sprite;

namespace PlayerContext
{
	class PlayerUIManager
	{
	public:
		void Initialize(Player* player);
		void Draw();
		void ImGuiDraw();
	private:
		void HPUI();
		void EnergyUI();
	private:
		Player* player_ = nullptr;

		Sprite* reticle_ = nullptr;

		// HPのUI
		GameUI::BarRenderInfo hpUI_{};
		// EnergyのUI
		GameUI::BarRenderInfo energyUI_{};
	private:
		GlobalVariables* globalVariables_ = nullptr;

	};
}

