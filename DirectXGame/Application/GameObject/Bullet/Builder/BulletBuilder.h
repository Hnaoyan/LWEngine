#pragma once
#include "../IBullet.h"
#include "../BulletEnums.h"

class BulletBuilder
{
public:
	BulletBuilder& SetType(TrackingType type) { trackType_ = type; }
	BulletBuilder& SetDirect(const EulerTransform& transform) { transform_ = transform; }
	BulletBuilder& SetDirect(const Vector3& direct) { direct_ = direct; }
	BulletBuilder& SetSpeed(const float& speed) { speed_ = speed; }

	std::unique_ptr<IBullet> Build(BulletType bulletType);

private:
	IGameObject* object_ = nullptr;
	// 追尾の種類
	TrackingType trackType_ = TrackingType::kStandard;
	EulerTransform transform_{};
	Vector3 direct_{};
	float speed_ = 0.0f;
};
