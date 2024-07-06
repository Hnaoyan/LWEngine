#pragma once
#include <variant>

class IGameObject;
class Player;
class SampleBullet;
class SampleEnemy;

using ColliderObject = std::variant<IGameObject*, Player*, SampleBullet*, SampleEnemy*>;
