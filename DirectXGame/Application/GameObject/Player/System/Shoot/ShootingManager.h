#pragma once
#include "Engine/Math/Vector/Vector3.h"
#include "Application/GameObject/Bullet/BulletEnums.h"

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
		// 初期化
		void Initialize(Player* player);
		// 更新
		void Update();
		// 通常射撃
		void OnFire(const Vector3& direct);
		// 誘導射撃
		void TrackingFire();
		// 誘導の生成
		void GenerateTracking(const Vector3& direct, TrackingAttribute type);
	public:
		void SetAttackPattern(bool isAttack) { isChangeAttack_ = isAttack; }

	private:
		// 撃つ種類
		ShootType type_ = ShootType::kDefault;
		// 射撃の変化
		bool isChangeAttack_ = false;
	private:
		Player* player_ = nullptr;
		BulletManager* bulletManager_ = nullptr;
	public:
		void SetManager(BulletManager* manager) { bulletManager_ = manager; }
	};

}
