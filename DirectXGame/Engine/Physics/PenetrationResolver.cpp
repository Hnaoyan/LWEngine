#include "PenetrationResolver.h"

Vector3 PenetrationResolver::ExtrusionCalculation(AABB mover, AABB obstacle)
{
    float overlapX1 = mover.max_.x - obstacle.min_.x;  // 右から
    float overlapX2 = mover.min_.x - obstacle.max_.x;  // 左から
    float overlapX = (overlapX1 < overlapX2) ? overlapX1 : -overlapX2;

    float overlapY1 = mover.max_.y - obstacle.min_.y;  // 上から
    float overlapY2 = mover.min_.y - obstacle.max_.y;  // 下から
    float overlapY = (overlapY1 < overlapY2) ? overlapY1 : -overlapY2;

    float overlapZ1 = mover.max_.z - obstacle.min_.z;  // 前から
    float overlapZ2 = mover.min_.z - obstacle.max_.z;  // 後から
    float overlapZ = (overlapZ1 < overlapZ2) ? overlapZ1 : -overlapZ2;

    // 最小の押し出し軸を選択
    float absX = std::abs(overlapX);
    float absY = std::abs(overlapY);
    float absZ = std::abs(overlapZ);

    if (absX <= absY && absX <= absZ) {
        return Vector3(overlapX, 0.0f, 0.0f);
    }
    else if (absY <= absX && absY <= absZ) {
        return Vector3(0.0f, overlapY, 0.0f);
    }
    else {
        return Vector3(0.0f, 0.0f, overlapZ);
    }
}

Vector2 PenetrationResolver::Extrusion2DCalculation(Rectangle2D mover, Rectangle2D obstacle)
{
    // X方向の重なり量
    float overlapX = (std::min)(mover.max_.x - obstacle.min_.x,
        obstacle.max_.x - mover.min_.x);
    // Y方向の重なり量
    float overlapY = (std::min)((float)mover.max_.y - obstacle.min_.y,
        (float)obstacle.max_.y - mover.min_.y);

    // 方向は中心差で決める
    if (mover.center_.x < obstacle.center_.x) {
        overlapX = -overlapX; // moverが左側 → 左に押し戻す
    }
    if (mover.center_.y < obstacle.center_.y) {
        overlapY = -overlapY; // moverが下側 → 下に押し戻す
    }

    // それぞれの軸の辺が揃っているか判定
    bool alignedX = (mover.max_.x == obstacle.max_.x) || (mover.min_.x == obstacle.min_.x);
    bool alignedY = (mover.max_.y == obstacle.max_.y) || (mover.min_.y == obstacle.min_.y);
    // チェック
    if (alignedY && !alignedX) return Vector2(overlapX, 0.0f);
    if (alignedX && !alignedY) return Vector2(0.0f, overlapY);

    // 押し出し軸を決定
    if (std::abs(overlapX) < std::abs(overlapY)) {
        return Vector2(overlapX, 0.0f);
    }
    else {
        return Vector2(0.0f, overlapY);
    }
}