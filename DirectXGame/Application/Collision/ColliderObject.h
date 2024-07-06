#pragma once
#include <variant>

class IGameObject;
class Player;
class SampleBullet;

using ColliderObject = std::variant<IGameObject*, Player*, SampleBullet*>;
