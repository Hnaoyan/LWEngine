#include "ITrackAttribute.h"
#include "../BulletsPaths.h"
#include <cassert>

void ITrackiAttribute::Initialize(IBullet* bullet)
{
	assert(bullet);
	bullet_ = bullet;

	// 加速度計算クラス生成
	accelerater_ = std::make_unique<TrackingAccelerater>(dynamic_cast<TrackingBullet*>(bullet_));
}

void ITrackiAttribute::Update()
{

}
