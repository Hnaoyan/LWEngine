#pragma once
#include "Engine/GlobalVariables/GlobalVariables.h"

class Player;

namespace PlayerContext
{
	class ISystem
	{
	public:
		virtual ~ISystem() = default;
	public:
		virtual void Initialize(Player* player) {
			player_ = player;
			globalVariables_ = GlobalVariables::GetInstance();
		}
		virtual void Update() = 0;
	protected:
		Player* player_ = nullptr;
		GlobalVariables* globalVariables_ = nullptr;
	};
}
