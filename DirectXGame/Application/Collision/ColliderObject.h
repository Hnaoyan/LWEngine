#pragma once
#include <variant>

class IGameObject;
class Player;
class IBullet;
class SampleEnemy;
class Terrain;
class PlayerFootCollider;

using ColliderObject = std::variant<IGameObject*, Player*, IBullet*, SampleEnemy*, Terrain*, PlayerFootCollider*>;
