#pragma once

class Player;

namespace PlayerContext
{
	class ISystem
	{
	public:
		virtual ~ISystem() = default;
	public:
		virtual void Initialize(Player* player) { player_ = player; }
		virtual void Update() = 0;
	protected:
		Player* player_ = nullptr;
	};
}
