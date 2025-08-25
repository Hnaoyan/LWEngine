#pragma once
#include <variant>

// 前方宣言
class IGameObject;
class Player;
class Obstacle;
class GoalObject;

// 衝突オブジェクトの登録
using ColliderObject = std::variant<IGameObject*, Player*, Obstacle*, GoalObject*>;
