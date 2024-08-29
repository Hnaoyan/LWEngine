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
	private:
		Player* player_ = nullptr;

		Sprite* reticle_ = nullptr;

		// HPのUI
		GameUI::BarRenderInfo hpUI_;

	};


}

