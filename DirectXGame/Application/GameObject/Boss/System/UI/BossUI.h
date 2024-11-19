#pragma once
#include "Application/GameSystem/UI/UIStructs.h"

class Boss;
class Sprite;

namespace BossSystemContext
{
	/// <summary>
	/// ボスのUI関係
	/// </summary>
	class BossUIManager {
	public:
		void Initialize(Boss* boss);
		void Draw();
		void ImGuiDraw();

	private:
		Boss* boss_ = nullptr;
		// HPのUI
		GameUI::BarRenderInfo hpUI_;

	};

}
