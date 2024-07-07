#pragma once
#include <variant>

class IGameObject;
class Player;
class SampleBullet;
class SampleEnemy;
class Terrain;

using ColliderObject = std::variant<IGameObject*, Player*, SampleBullet*, SampleEnemy*, Terrain*>;
