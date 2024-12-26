#include "BulletBuilder.h"
#include "Engine/Collision/Collider/ICollider.h"
#include "Application/Collision/ColliderFilter.h"
#include "../../BulletsPaths.h"
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
		static_cast<TrackingBullet*>(instance.get())->SetTrackType(trackAttribute_);	// 属性の設定
		static_cast<TrackingBullet*>(instance.get())->SetIsBarrage(isRandStraight_);	// 直進時間ランダムフラグ
		static_cast<TrackingBullet*>(instance.get())->SetStraightFrame(straightFrame_);
		transform.scale = global->GetValue<Vector3>("BossTrackingBullet", "Scale");
		break;
	case BulletType::kContainer:
		instance = std::make_unique<ContainerBullet>();
		//ContainerBullet* bullet = static_cast<ContainerBullet*>(instance.get());
		static_cast<ContainerBullet*>(instance.get())->SetGameObject(object_);
		static_cast<ContainerBullet*>(instance.get())->SetCluster(cluster_);
		break;
	default:
		break;
	}
	// 速度の設定
	instance->SetVelocity(direct_ * speed_);
	// 初期化
	instance->Initialize();
	instance->transform_ = transform;
	if (parentAttribute_ == 0) {
		instance->GetCollider()->SetAttribute(kCollisionAttributeEnemyBullet);
	}
	else {
		instance->GetCollider()->SetAttribute(kCollisionAttributeBullet);
	}

	// 返す
	return instance;
}