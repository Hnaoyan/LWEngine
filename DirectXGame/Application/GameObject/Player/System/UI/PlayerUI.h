#pragma once
#include "Application/GameSystem/UI/GameUIManager.h"

class Player;
class Sprite;

namespace PlayerContext
{
	class UIManager
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
	};


}

