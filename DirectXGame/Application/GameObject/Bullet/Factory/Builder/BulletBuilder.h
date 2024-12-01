#pragma once
#include "../../IBullet.h"
#include "../../BulletEnums.h"

#include <optional>

class IBulletCluster;

/// <summary>
/// 生成のパラメータ設定クラス（ビルダークラス
/// </summary>
class BulletBuilder
{
public: // 設定
	// 追従の種類設定
	BulletBuilder& SetAttribute(TrackingAttribute type) { 
		trackAttribute_ = type;
		return *this;
	}
	// トランスフォーム設定
	BulletBuilder& SetTransform(const EulerTransform& transform) { 
		transform_ = transform;
		return *this;
	}
	// 方向設定
	BulletBuilder& SetDirect(const Vector3& direct) { 
		direct_ = Vector3::Normalize(direct);
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
	BulletBuilder& SetIsRandStraight(bool flag) {
		isRandStraight_ = flag;
		return *this;
	}
	BulletBuilder& SetCluster(IBulletCluster* cluster) {
		cluster_ = cluster;
		return *this;
	}
	BulletBuilder& SetParentAttribute(int32_t attribute) {
		parentAttribute_ = attribute;
		return *this;
	}
	//// 直進の時間設定
	//BulletBuilder& SetStraightFrame(const float& frame) {
	//	straightRandomer_ = frame; 
	//	return *this;
	//}

public:
	/// <summary>
	/// 生成関数
	/// </summary>
	/// <param name="bulletType"></param>
	/// <returns></returns>
	std::unique_ptr<IBullet> Build(BulletType bulletType) const;

	TrackingAttribute GetAttribute() const { return trackAttribute_; }

private:
	// 追従の対象
	IGameObject* object_ = nullptr;
	// 追尾の種類
	TrackingAttribute trackAttribute_ = TrackingAttribute::kSuperior;
	// 自機のトランスフォーム
	EulerTransform transform_{};
	// 進行方向
	Vector3 direct_{};
	// 速度のレシオ
	float speed_ = 0.0f;
	// 直進時間のランダムの有無
	bool isRandStraight_ = false;
	// 直進のタイマーの幅
	std::optional<float> straightRandomer_;
	// 
	IBulletCluster* cluster_ = nullptr;
	// 親の属性（０なら敵・１ならプレイヤー
	int32_t parentAttribute_ = 0u;

};
