#pragma once

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
		Sprite* health_ = nullptr;

	};


}

