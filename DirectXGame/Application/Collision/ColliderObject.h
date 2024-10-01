#pragma once
#include <variant>

// 前方宣言
class IGameObject;
class Player;
class IBullet;
class SampleEnemy;
class Boss;
class Terrain;
class PlayerFootCollider;
namespace BossSystemContext {
	class NormalBullet;
	class TrackingBullet;
}

// 衝突オブジェクトの登録
using ColliderObject = std::variant<IGameObject*, Player*, SampleEnemy*, Boss*, Terrain*,
	PlayerFootCollider*, IBullet*, BossSystemContext::NormalBullet*, BossSystemContext::TrackingBullet*>;
