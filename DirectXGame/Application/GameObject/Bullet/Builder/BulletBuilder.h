#pragma once
#include "../IBullet.h"
#include "../BulletEnums.h"

class BulletBuilder
{
public: // 設定
	// 追従の種類設定
	BulletBuilder& SetType(TrackingAttribute type) { 
		trackType_ = type;
		return *this;
	}
	// トランスフォーム設定
	BulletBuilder& SetTransform(const EulerTransform& transform) { 
		transform_ = transform;
		return *this;
	}
	// 方向設定
	BulletBuilder& SetDirect(const Vector3& direct) { 
		direct_ = direct;
		return *this;
	}
	// 速度設定
	BulletBuilder& SetSpeed(const float& speed) { 
		speed_ = speed;
		return *this;
	}
	// 対象の設定
	BulletBuilder& SetTargetObject(IGameObject* object) {
		object_ = object;
		return *this;
	}
public:
	/// <summary>
	/// 生成関数
	/// </summary>
	/// <param name="bulletType"></param>
	/// <returns></returns>
	std::unique_ptr<IBullet> Build(BulletType bulletType) const;

private:
	// 追従の対象
	IGameObject* object_ = nullptr;
	// 追尾の種類
	TrackingAttribute trackType_ = TrackingAttribute::kSuperior;
	// 自機のトランスフォーム
	EulerTransform transform_{};
	// 進行方向
	Vector3 direct_{};
	// 速度のレシオ
	float speed_ = 0.0f;
};
