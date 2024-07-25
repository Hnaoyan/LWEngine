#include "Collision.h"
#include <cmath>
#include <algorithm>

bool Collision::IsCollision(const AABB& aabb1, const AABB& aabb2)
{
	if ((aabb1.min_.x <= aabb2.max_.x && aabb1.max_.x >= aabb2.min_.x) &&	// X軸
		(aabb1.min_.y <= aabb2.max_.y && aabb1.max_.y >= aabb2.min_.y) &&	// Y軸
		(aabb1.min_.z <= aabb2.max_.z && aabb1.max_.z >= aabb2.min_.z)) {	// Z軸
		return true;
	}
	return false;
}

bool Collision::IsCollision(const AABB& aabb, const Sphere& sphere)
{
	// 最近接点を求める
	Vector3 closestPoint{ std::clamp(sphere.worldTransform_.GetWorldPosition().x,aabb.min_.x,aabb.max_.x),
		std::clamp(sphere.worldTransform_.GetWorldPosition().y,aabb.min_.y,aabb.max_.y),
		std::clamp(sphere.worldTransform_.GetWorldPosition().z,aabb.min_.z,aabb.max_.z) };

	// 最近接点と球の中心との距離を求める
	float distance = Vector3::Length(closestPoint - sphere.worldTransform_.GetWorldPosition());
	// 距離が半径よりも小さければ衝突
	if (distance <= sphere.radius_) {
		return true;
	}

	return false;
}

bool Collision::IsCollision(const AABB& aabb, const OBB& obb)
{
	aabb, obb;
	return false;
}

bool Collision::IsCollision(const Sphere& sphere, const AABB& aabb)
{
	// 最近接点を求める
	Vector3 closestPoint{ std::clamp(sphere.worldTransform_.GetWorldPosition().x,aabb.min_.x,aabb.max_.x),
		std::clamp(sphere.worldTransform_.GetWorldPosition().y,aabb.min_.y,aabb.max_.y),
		std::clamp(sphere.worldTransform_.GetWorldPosition().z,aabb.min_.z,aabb.max_.z) };

	// 最近接点と球の中心との距離を求める
	float distance = Vector3::Length(closestPoint - sphere.worldTransform_.GetWorldPosition());
	// 距離が半径よりも小さければ衝突
	if (distance <= sphere.radius_) {
		return true;
	}

	return false;
}

bool Collision::IsCollision(const Sphere& sphere1, const Sphere& sphere2)
{
	// 2球体の距離を求める
	float distance = Vector3::Length(sphere1.worldTransform_.GetWorldPosition() - sphere2.worldTransform_.GetWorldPosition());
	// 半径の合計よりも短いか
	if (distance <= sphere1.radius_ + sphere2.radius_) {
		return true;
	}
	return false;
}

bool Collision::IsCollision(const Sphere& sphere, const OBB& obb)
{
	sphere, obb;
	return false;
}

bool Collision::IsCollision(const OBB& obb1, const OBB& obb2)
{
	obb1, obb2;
	// 各方向ベクトルの確保
	//	// （N***:標準化方向ベクトル）
	//Vector3 NAe1 = obb1.normalDirect[0], Ae1 = NAe1 * obb1.length_.x;
	//Vector3 NAe2 = obb1.normalDirect[1], Ae2 = (NAe2* obb1.length_.y);
	//Vector3 NAe3 = obb1.normalDirect[2], Ae3 = (NAe3 * obb1.length_.z);
	//Vector3 NBe1 = obb2.normalDirect[0], Be1 = (obb2.length_.x, NBe1);
	//Vector3 NBe2 = obb2.normalDirect[1], Be2 = (obb2.length_.y, NBe2);
	//Vector3 NBe3 = obb2.normalDirect[2], Be3 = (obb2.length_.z, NBe3);
	//Vector3 Interval = Subtract(obb1.m_Pos, obb2.m_Pos);

	//// 分離軸 : Ae1
	//float rA = Length(Ae1);
	//float rB = LenSegOnSeparateAxis(&NAe1, &Be1, &Be2, &Be3);
	//float L = fabsf(Dot(Interval, NAe1));
	//if (L > rA + rB) {
	//	return false; // 衝突していない
	//}

	//// 分離軸 : Ae2
	//rA = Length(Ae2);
	//rB = LenSegOnSeparateAxis(&NAe2, &Be1, &Be2, &Be3);
	//L = fabsf(Dot(Interval, NAe2));
	//if (L > rA + rB) {
	//	return false;
	//}

	//// 分離軸 : Ae3
	//rA = Length(Ae3);
	//rB = LenSegOnSeparateAxis(&NAe3, &Be1, &Be2, &Be3);
	//L = fabsf(Dot(Interval, NAe3));
	//if (L > rA + rB) {
	//	return false;
	//}

	//// 分離軸 : Be1
	//rA = LenSegOnSeparateAxis(&NBe1, &Ae1, &Ae2, &Ae3);
	//rB = Length(Be1);
	//L = fabsf(Dot(Interval, NBe1));
	//if (L > rA + rB) {
	//	return false;
	//}

	//// 分離軸 : Be2
	//rA = LenSegOnSeparateAxis(&NBe2, &Ae1, &Ae2, &Ae3);
	//rB = Length(Be2);
	//L = fabsf(Dot(Interval, NBe2));
	//if (L > rA + rB) {
	//	return false;
	//}

	//// 分離軸 : Be3
	//rA = LenSegOnSeparateAxis(&NBe3, &Ae1, &Ae2, &Ae3);
	//rB = Length(Be3);
	//L = fabsf(Dot(Interval, NBe3));
	//if (L > rA + rB) {
	//	return false;
	//}

	//// 分離軸 : C11
	//Vector3 cross;
	//cross = Cross(NAe1, NBe1);
	//rA = LenSegOnSeparateAxis(&cross, &Ae2, &Ae3);
	//rB = LenSegOnSeparateAxis(&cross, &Be2, &Be3);
	//L = fabsf(Dot(Interval, cross));
	//if (L > rA + rB) {
	//	return false;
	//}

	//// 分離軸 : C12
	//cross = Cross(NAe1, NBe2);
	//rA = LenSegOnSeparateAxis(&cross, &Ae2, &Ae3);
	//rB = LenSegOnSeparateAxis(&cross, &Be1, &Be3);
	//L = fabsf(Dot(Interval, cross));
	//if (L > rA + rB) {
	//	return false;
	//}

	//// 分離軸 : C13
	//cross = Cross(NAe1, NBe3);
	//rA = LenSegOnSeparateAxis(&cross, &Ae2, &Ae3);
	//rB = LenSegOnSeparateAxis(&cross, &Be1, &Be2);
	//L = fabsf(Dot(Interval, cross));
	//if (L > rA + rB) {
	//	return false;
	//}

	//// 分離軸 : C21
	//cross = Cross(NAe2, NBe1);
	//rA = LenSegOnSeparateAxis(&cross, &Ae1, &Ae3);
	//rB = LenSegOnSeparateAxis(&cross, &Be2, &Be3);
	//L = fabsf(Dot(Interval, cross));
	//if (L > rA + rB) {
	//	return false;
	//}

	//// 分離軸 : C22
	//cross = Cross(NAe2, NBe2);
	//rA = LenSegOnSeparateAxis(&cross, &Ae1, &Ae3);
	//rB = LenSegOnSeparateAxis(&cross, &Be1, &Be3);
	//L = fabsf(Dot(Interval, cross));
	//if (L > rA + rB) {
	//	return false;
	//}

	//// 分離軸 : C23
	//cross = Cross(NAe2, NBe3);
	//rA = LenSegOnSeparateAxis(&cross, &Ae1, &Ae3);
	//rB = LenSegOnSeparateAxis(&cross, &Be1, &Be2);
	//L = fabsf(Dot(Interval, cross));
	//if (L > rA + rB) {
	//	return false;
	//}

	//// 分離軸 : C31
	//cross = Cross(NAe3, NBe1);
	//rA = LenSegOnSeparateAxis(&cross, &Ae1, &Ae2);
	//rB = LenSegOnSeparateAxis(&cross, &Be2, &Be3);
	//L = fabsf(Dot(Interval, cross));
	//if (L > rA + rB) {
	//	return false;
	//}

	//// 分離軸 : C32
	//cross = Cross(NAe3, NBe2);
	//rA = LenSegOnSeparateAxis(&cross, &Ae1, &Ae2);
	//rB = LenSegOnSeparateAxis(&cross, &Be1, &Be3);
	//L = fabsf(Dot(Interval, cross));
	//if (L > rA + rB) {
	//	return false;
	//}

	//// 分離軸 : C33
	//cross = Cross(NAe3, NBe3);
	//rA = LenSegOnSeparateAxis(&cross, &Ae1, &Ae2);
	//rB = LenSegOnSeparateAxis(&cross, &Be1, &Be2);
	//L = fabsf(Dot(Interval, cross));
	//if (L > rA + rB) {
	//	return false;
	//}

	// 分離平面が存在しないので「衝突している」
	return true;
}

bool Collision::IsCollision(const OBB& obb, const Sphere& sphere)
{
	obb, sphere;
	return false;
}

bool Collision::IsCollision(const OBB& obb, const AABB& aabb)
{
	obb, aabb;
	return false;
}
