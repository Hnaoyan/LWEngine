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
		/// <summary>
		/// 初期化
		/// </summary>
		/// <param name="player"></param>
		void Initialize(Player* player);
		/// <summary>
		/// 描画
		/// </summary>
		void Draw();
		/// <summary>
		/// ImGui
		/// </summary>
		void ImGuiDraw();
	private:
		/// <summary>
		/// HPUI関係
		/// </summary>
		void HPUI();
		/// <summary>
		/// EnergyUI関係
		/// </summary>
		void EnergyUI();
	private:
		// 親
		Player* player_ = nullptr;
		// レティクル用スプライト
		Sprite* reticle_ = nullptr;
		// HPのUI
		GameUI::BarRenderInfo hpUI_{};
		// EnergyのUI
		GameUI::BarRenderInfo energyUI_{};
	private:
		GlobalVariables* globalVariables_ = nullptr;

	};
}

