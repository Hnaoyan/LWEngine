#pragma once
#include "../IBullet.h"
#include "../BulletEnums.h"

class BulletBuilder
{
public:
	BulletBuilder& SetType(TrackingType type) { 
		trackType_ = type;
		return *this;
	}
	BulletBuilder& SetTransform(const EulerTransform& transform) { 
		transform_ = transform;
		return *this;
	}
	BulletBuilder& SetDirect(const Vector3& direct) { 
		direct_ = direct;
		return *this;
	}
	BulletBuilder& SetSpeed(const float& speed) { 
		speed_ = speed;
		return *this;
	}
	BulletBuilder& SetTargetObject(IGameObject* object) {
		object_ = object;
		return *this;
	}

	std::unique_ptr<IBullet> Build(BulletType bulletType) const;

private:
	// 追従の対象
	IGameObject* object_ = nullptr;
	// 追尾の種類
	TrackingType trackType_ = TrackingType::kSuperior;
	// 自機のトランスフォーム
	EulerTransform transform_{};
	// 進行方向
	Vector3 direct_{};
	// 速度のレシオ
	float speed_ = 0.0f;
};
