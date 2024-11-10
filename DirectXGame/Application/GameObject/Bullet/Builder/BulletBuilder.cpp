#include "BulletBuilder.h"
#include "../BulletsLists.h"
#include "Engine/GlobalVariables/GlobalVariables.h"

std::unique_ptr<IBullet> BulletBuilder::Build(BulletType bulletType) const
{
	GlobalVariables* global = GlobalVariables::GetInstance();

	std::unique_ptr<IBullet> instance;
	EulerTransform transform = transform_;

	// 生成の種類ごとのやつ
	switch (bulletType)
	{
	case BulletType::kNormal:
		instance = std::make_unique<NormalSpinBullet>();
		transform.scale = global->GetValue<Vector3>("BossNormalBullet", "Scale");
		break;
	case BulletType::kTracking:
		instance = std::make_unique<TrackingBullet>();
		static_cast<TrackingBullet*>(instance.get())->SetGameObject(object_);
		static_cast<TrackingBullet*>(instance.get())->SetTrackType(trackType_);	// 属性の設定
		static_cast<TrackingBullet*>(instance.get())->SetIsBarrage(isRandStraight_);	// 直進時間ランダムフラグ
		transform.scale = global->GetValue<Vector3>("BossTrackingBullet", "Scale");
		break;
	default:
		break;
	}
	// 初期化
	instance->Initialize();
	// 速度の設定
	instance->SetVelocity(direct_ * speed_);
	instance->transform_ = transform;

	// 返す
	return instance;
}