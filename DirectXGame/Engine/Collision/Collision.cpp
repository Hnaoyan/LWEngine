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
