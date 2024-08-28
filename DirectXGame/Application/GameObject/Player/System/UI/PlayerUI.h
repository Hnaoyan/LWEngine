#pragma once

class Player;

namespace PlayerContext
{
	class UIManager
	{
	public:
		void Initialize(Player* player);
		void Draw();
	private:
		Player* player_ = nullptr;

	};


}

