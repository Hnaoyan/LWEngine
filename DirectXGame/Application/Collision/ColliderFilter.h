#pragma once
#include <cstdint>

//プレイヤー
inline static constexpr uint32_t kCollisionAttributePlayer = 0b1;
// 地形
inline static constexpr uint32_t kCollisionAttributeTerrain = 0b1 << 1;
// 敵
inline static constexpr uint32_t kCollisionAttributeEnemy = 0b1 << 2;
// 足場コライダー
inline static constexpr uint32_t kCollisionAttributeFootCollider = 0b1 << 3;
// 弾
inline static constexpr uint32_t kCollisionAttributeBullet = 0b1 << 4;
// 敵の弾
inline static constexpr uint32_t kCollisionAttributeEnemyBullet = 0b1 << 5;