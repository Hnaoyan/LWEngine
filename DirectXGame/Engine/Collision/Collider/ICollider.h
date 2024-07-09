#pragma once
#include "../../3D/WorldTransform.h"
#include "ColliderShape.h"
#include "Application/Collision/ColliderObject.h"

class ICollider
{
public:
	struct BoxVertices {
		Vector3 leftBottomFront, leftBottomBack, leftTopFront, leftTopBack;
		Vector3 rightBottomFront, rightBottomBack, rightTopFront, rightTopBack;
	};
	enum CollisionType3D {
		// 8頂点（四隅
		kLeftBottomFront,kLeftBottomBack,kLeftTopFront,kLeftTopBack,
		kRightBottomFront, kRightBottomBack, kRightTopFront, kRightTopBack,

		kMultiplePoints,
		kNone,
	};

	inline static BoxVertices CreateBoxVertices(const Vector3& min, const Vector3& max) {
		BoxVertices result{};
		// 左下
		result.leftBottomFront = { min.x,min.y,min.z };
		result.leftBottomBack = { min.x,min.y,max.z };
		// 左上
		result.leftTopFront = { min.x,max.y,min.z };
		result.leftTopBack = { min.x,max.y,max.z };
		// 右下
		result.rightBottomFront = { max.x,min.y,min.z };
		result.rightBottomBack = { max.x,min.y,max.z };
		// 右上
		result.rightTopFront = { max.x,max.y,min.z };
		result.rightTopBack = { max.x,max.y,max.z };

		return result;
	}

	inline static CollisionType3D GetCollisionType3D(BoxVertices box, const Vector3& targetMin, const Vector3& targetMax) {
		// 各頂点がターゲットAABB内にあるかをチェック
		bool isInLeftBottomFront = (box.leftBottomFront.x > targetMin.x && box.leftBottomFront.x < targetMax.x &&
			box.leftBottomFront.y > targetMin.y && box.leftBottomFront.y < targetMax.y &&
			box.leftBottomFront.z > targetMin.z && box.leftBottomFront.z < targetMax.z);

		bool isInLeftBottomBack = (box.leftBottomBack.x > targetMin.x && box.leftBottomBack.x < targetMax.x &&
			box.leftBottomBack.y > targetMin.y && box.leftBottomBack.y < targetMax.y &&
			box.leftBottomBack.z > targetMin.z && box.leftBottomBack.z < targetMax.z);

		bool isInLeftTopFront = (box.leftTopFront.x > targetMin.x && box.leftTopFront.x < targetMax.x &&
			box.leftTopFront.y > targetMin.y && box.leftTopFront.y < targetMax.y &&
			box.leftTopFront.z > targetMin.z && box.leftTopFront.z < targetMax.z);

		bool isInLeftTopBack = (box.leftTopBack.x > targetMin.x && box.leftTopBack.x < targetMax.x &&
			box.leftTopBack.y > targetMin.y && box.leftTopBack.y < targetMax.y &&
			box.leftTopBack.z > targetMin.z && box.leftTopBack.z < targetMax.z);

		bool isInRightBottomFront = (box.rightBottomFront.x > targetMin.x && box.rightBottomFront.x < targetMax.x &&
			box.rightBottomFront.y > targetMin.y && box.rightBottomFront.y < targetMax.y &&
			box.rightBottomFront.z > targetMin.z && box.rightBottomFront.z < targetMax.z);

		bool isInRightBottomBack = (box.rightBottomBack.x > targetMin.x && box.rightBottomBack.x < targetMax.x &&
			box.rightBottomBack.y > targetMin.y && box.rightBottomBack.y < targetMax.y &&
			box.rightBottomBack.z > targetMin.z && box.rightBottomBack.z < targetMax.z);

		bool isInRightTopFront = (box.rightTopFront.x > targetMin.x && box.rightTopFront.x < targetMax.x &&
			box.rightTopFront.y > targetMin.y && box.rightTopFront.y < targetMax.y &&
			box.rightTopFront.z > targetMin.z && box.rightTopFront.z < targetMax.z);

		bool isInRightTopBack = (box.rightTopBack.x > targetMin.x && box.rightTopBack.x < targetMax.x &&
			box.rightTopBack.y > targetMin.y && box.rightTopBack.y < targetMax.y &&
			box.rightTopBack.z > targetMin.z && box.rightTopBack.z < targetMax.z);

		// 頂点の数をカウント
		int inSideCount = isInLeftBottomFront + isInLeftBottomBack + isInLeftTopFront + isInLeftTopBack +
			isInRightBottomFront + isInRightBottomBack + isInRightTopFront + isInRightTopBack;

		// 各条件に基づいて衝突タイプを返す
		if (inSideCount == 1) {
			if (isInLeftBottomFront) {
				return CollisionType3D::kLeftBottomFront;
			}
			if (isInLeftBottomBack) {
				return CollisionType3D::kLeftBottomBack;
			}
			if (isInLeftTopFront) {
				return CollisionType3D::kLeftTopFront;
			}
			if (isInLeftTopBack) {
				return CollisionType3D::kLeftTopBack;
			}
			if (isInRightBottomFront) {
				return CollisionType3D::kRightBottomFront;
			}
			if (isInRightBottomBack) {
				return CollisionType3D::kRightBottomBack;
			}
			if (isInRightTopFront) {
				return CollisionType3D::kRightTopFront;
			}
			if (isInRightTopBack) {
				return CollisionType3D::kRightTopBack;
			}
		}
		else if (inSideCount >= 3) {
			return CollisionType3D::kMultiplePoints;
		}

		return CollisionType3D::kNone;
	}
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="shape"></param>
	virtual void Initialize(ColliderObject object);
	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update(const Vector3& worldPosition);

public:
	/// <summary>
	/// 属性取得
	/// </summary>
	/// <returns></returns>
	uint32_t GetAttribute() { return collisionAttribute_; }
	/// <summary>
	/// マスク取得
	/// </summary>
	/// <returns></returns>
	uint32_t GetMask() { return collisionMask_; }
	/// <summary>
	/// 属性設定
	/// </summary>
	/// <param name="attribute"></param>
	void SetAttribute(uint32_t attribute) { collisionAttribute_ = attribute; }
	/// <summary>
	/// マスク設定
	/// </summary>
	/// <param name="mask"></param>
	void SetMask(uint32_t mask) { collisionMask_ = mask; }
	/// <summary>
	/// クラスの取得
	/// </summary>
	/// <returns></returns>
	ColliderObject GetColliderObject() { return objectRegistry_; }
protected:
	// 衝突属性（自分）
	uint32_t collisionAttribute_ = 0xffffffff;
	// 衝突マスク
	uint32_t collisionMask_ = 0xffffffff;
	// 自分のオブジェクトタイプ
	ColliderObject objectRegistry_;

public:
	// ワールドトランスフォーム
	WorldTransform worldTransform_{};
};
