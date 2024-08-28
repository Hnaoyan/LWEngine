#pragma once
#include <stdint.h>

class Player;

namespace PlayerContext
{
	class AssendingManager
	{
	public:
		void Initialize(Player* player);
		void Update();

	private:
		Player* player_ = nullptr;

	};


}
