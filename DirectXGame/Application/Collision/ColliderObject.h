#pragma once
#include <variant>

class IGameObject;
class Player;

using ColliderObject = std::variant<IGameObject*, Player*>;
