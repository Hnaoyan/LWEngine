#pragma once
#include <variant>

// 前方宣言
class IGameObject;
class Player;
class Obstacle;

// 衝突オブジェクトの登録
using ColliderObject = std::variant<IGameObject*, Player*, Obstacle*>;
