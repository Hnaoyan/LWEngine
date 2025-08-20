#include "CollisionManager.h"
#include "Collider/ColliderLists.h"
#include "Collision.h"
#include "2D/Collider2DLists.h"
#include "2D/Collision2D.h"
#include "Application/GameObject/GameObjectLists.h"

void CollisionManager::CheckAllCollisions()
{
	// リストチェック
	std::list<ColliderShape>::iterator itrA = colliders_.begin();
	for (; itrA != colliders_.end(); ++itrA) {
		ColliderShape colliderA = *itrA;
		
		// イテレータBはAの次の要素から回す
		std::list<ColliderShape>::iterator itrB = itrA;
		itrB++;

		for (; itrB != colliders_.end(); ++itrB) {
			ColliderShape colliberB = *itrB;
			// 衝突チェック
			CheckCollisionPair(colliderA, colliberB);
		}
	}

	// リストチェック
	std::list<Collider2DShape>::iterator itrA2D = colliders2D_.begin();
	for (; itrA2D != colliders2D_.end(); ++itrA2D) {
		Collider2DShape colliderA = *itrA2D;

		// イテレータBはAの次の要素から回す
		std::list<Collider2DShape>::iterator itrB = itrA2D;
		itrB++;

		for (; itrB != colliders2D_.end(); ++itrB) {
			Collider2DShape colliberB = *itrB;
			// 衝突チェック
			CheckCollisionPair(colliderA, colliberB);
		}
	}
}

void CollisionManager::ListClear()
{
	// リストクリア
	colliders_.clear();
	colliders2D_.clear();
}

void CollisionManager::ListRegist(ColliderShape collider)
{
	// 追加
	colliders_.push_back(collider);
}

void CollisionManager::ListRegist(Collider2DShape collider)
{
	// 追加
	colliders2D_.push_back(collider);
}

void CollisionManager::CheckCollisionPair(ColliderShape colliderA, ColliderShape colliderB)
{
	// variantの中身を自動変換
	std::visit([](const auto& a, const auto& b) {
		// マスク処理
		// 同じマスク同士ならスキップ
		if (a->GetAttribute() == b->GetAttribute()) {
			return;
		}
		// 衝突処理
		if (Collision::IsCollision(*a, *b)) {

			std::visit([=](const auto& x, const auto& y) {
				x->OnCollision(y);
				y->OnCollision(x);
				}, a->GetColliderObject(), b->GetColliderObject());

		}

		}, colliderA, colliderB);

}

void CollisionManager::CheckCollisionPair(Collider2DShape colliderA, Collider2DShape colliderB)
{
	// variantの中身を自動変換
	std::visit([](const auto& a, const auto& b) {
		// マスク処理
		// 同じマスク同士ならスキップ
		if (a->GetAttribute() == b->GetAttribute()) {
			return;
		}
		// 衝突処理
		if (Collision2D::IsCollision(*a, *b)) {

			std::visit([=](const auto& x, const auto& y) {
				x->OnCollision(y);
				y->OnCollision(x);
				}, a->GetColliderObject(), b->GetColliderObject());

		}

		}, colliderA, colliderB);

}
