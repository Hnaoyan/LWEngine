#pragma once
#include "Application/GameSystem/UI/GameUIManager.h"

class Boss;
class Sprite;

namespace BossSystemContext
{

	class UIManager {
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
