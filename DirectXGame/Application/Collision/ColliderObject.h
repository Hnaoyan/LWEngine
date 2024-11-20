#pragma once
#include <variant>

// 前方宣言
class IGameObject;
class Player;
class IBullet;
class Boss;
class Terrain;
class PlayerFootCollider;

// 衝突オブジェクトの登録
using ColliderObject = std::variant<IGameObject*, Player*, Boss*, Terrain*,
	PlayerFootCollider*, IBullet*>;
