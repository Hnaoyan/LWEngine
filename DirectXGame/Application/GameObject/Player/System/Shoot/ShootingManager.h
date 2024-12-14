#pragma once
#include "Engine/Math/Vector/Vector3.h"
#include "Application/GameObject/Bullet/BulletEnums.h"
#include "../IPlayerSystem.h"

class Player;
class BulletManager;

/// <summary>
/// 射撃処理
/// </summary>
namespace PlayerContext
{
	enum class ShootType : uint32_t
	{
		kDefault,
		kHighSpeed,
		kSpread,
	};

	class ShootingManager : public ISystem
	{
	public:
		// 初期化
		void Initialize(Player* player) override;
		// 更新
		void Update() override;
		// 通常射撃
		void OnFire(const Vector3& direct);
		// 誘導射撃
		void TrackingFire();
		// 誘導の生成
		void GenerateTracking(const Vector3& direct, TrackingAttribute type);
		// 発射処理
		void FireContainer();

	public: // アクセッサ
		void SetAttackPattern(bool isAttack) { isChangeAttack_ = isAttack; }
		void SetManager(BulletManager* manager) { bulletManager_ = manager; }
	private:
		// 撃つ種類
		ShootType type_ = ShootType::kDefault;
		// 射撃の変化
		bool isChangeAttack_ = false;
	private:
		Player* player_ = nullptr;
		BulletManager* bulletManager_ = nullptr;
	};

}
