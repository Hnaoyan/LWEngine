#pragma once
#include <variant>

class IGameObject;
class Player;
class SampleBullet;
class IBullet;
class SampleEnemy;
class Terrain;
class PlayerFootCollider;

using ColliderObject = std::variant<IGameObject*, Player*, SampleBullet*, IBullet*, SampleEnemy*, Terrain*, PlayerFootCollider*>;
