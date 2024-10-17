#include "BulletBuilder.h"
#include "../BulletsLists.h"

std::unique_ptr<IBullet> BulletBuilder::Build(BulletType bulletType)
{
	std::unique_ptr<IBullet> instance;
	// 生成の種類ごとのやつ
	switch (bulletType)
	{
	case BulletType::kNormal:
		instance = std::make_unique<IBullet>();
		break;
	case BulletType::kTracking:
		instance = std::make_unique<TrackingBullet>();
		static_cast<TrackingBullet*>(instance.get())->SetGameObject(object_);
		static_cast<TrackingBullet*>(instance.get())->SetTrackType(trackType_);
		break;
	default:
		break;
	}
	// 初期化
	instance->Initialize();
	// 速度の設定
	instance->SetVelocity(direct_ * speed_);

	// 返す
	return instance;
}