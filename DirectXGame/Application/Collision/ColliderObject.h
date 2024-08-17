#pragma once
#include <variant>

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
using ColliderObject = std::variant<IGameObject*, Player*, SampleEnemy*, Boss*, Terrain*,
	PlayerFootCollider*, IBullet*, BossSystemContext::NormalBullet*, BossSystemContext::TrackingBullet*>;
