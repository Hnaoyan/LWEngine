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
}
using ColliderObject = std::variant<IGameObject*, Player*, IBullet*, SampleEnemy*, Boss*, Terrain*, PlayerFootCollider*, BossSystemContext::NormalBullet*>;
