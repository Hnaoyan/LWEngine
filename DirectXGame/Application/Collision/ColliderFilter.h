#pragma once
#include <cstdint>

//プレイヤー
inline static constexpr uint32_t kCollisionAttributePlayer = 0b1;
// 地形
inline static constexpr uint32_t kCollisionAttributeTerrain = 0b1 << 1;