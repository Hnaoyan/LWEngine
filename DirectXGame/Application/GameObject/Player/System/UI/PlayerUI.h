#pragma once
#include "Application/GameSystem/UI/UIStructs.h"
#include "Engine/GlobalVariables/GlobalVariables.h"
#include "../IPlayerSystem.h"

class Player;
class Sprite;

/// <summary>
/// プレイヤーに直接関係するＵＩ
/// </summary>
namespace PlayerContext
{
	class PlayerUIManager : public ISystem
	{
	public:
		/// <summary>
		/// 初期化
		/// </summary>
		/// <param name="player"></param>
		void Initialize(Player* player) override;

		void Update() override;
		/// <summary>
		/// 描画
		/// </summary>
		void Draw();
		/// <summary>
		/// ImGui
		/// </summary>
		void ImGuiDraw();
	public:
		
		void ComboGageUpdate();
		void Draw(ModelDrawDesc desc);
	private:
		/// <summary>
		/// HPUI関係
		/// </summary>
		void HPUI();
		/// <summary>
		/// EnergyUI関係
		/// </summary>
		void EnergyUI();
	public:
		GameUI::BillBoardRenderInfo* GetJustDodgeUI() { return &justDodgeUI_; }
	private:
		// レティクル用スプライト
		Sprite* reticle_ = nullptr;
		// HPのUI
		GameUI::BarRenderInfo hpUI_{};
		// EnergyのUI
		GameUI::BarRenderInfo energyUI_{};

		// ジャスト回避コンボのUI
		GameUI::BillBoardRenderInfo justDodgeUI_{};

	};
}

