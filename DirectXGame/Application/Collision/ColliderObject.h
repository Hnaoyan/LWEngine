#pragma once
#include <variant>

class IGameObject;
class Player;
class SampleBullet;
class SampleEnemy;
class Terrain;
class PlayerFootCollider;

using ColliderObject = std::variant<IGameObject*, Player*, SampleBullet*, SampleEnemy*, Terrain*, PlayerFootCollider*>;
