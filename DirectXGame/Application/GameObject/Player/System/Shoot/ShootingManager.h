#pragma once
#include "Engine/Math/Vector/Vector3.h"

class Player;
class BulletManager;

namespace PlayerContext
{
	enum class ShootType : uint32_t
	{
		kDefault,
		kHighSpeed,
		kSpread,
	};

	class ShootingManager
	{
	public:
		void Initialize(Player* player);
		void Update();
		void OnFire(const Vector3& direct);
	private:

	private:
		// 撃つ種類
		ShootType type_ = ShootType::kDefault;
	private:
		Player* player_ = nullptr;
		BulletManager* bulletManager_ = nullptr;
	public:
		void SetManager(BulletManager* manager) { bulletManager_ = manager; }
	};

}