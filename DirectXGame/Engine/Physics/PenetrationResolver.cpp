#include "PenetrationResolver.h"

Vector3 PenetrationResolver::ExtrusionCalculation(AABB mover, AABB obstacle)
{
    Vector3 max = mover.max_ - obstacle.max_;
    Vector3 min = mover.min_ - obstacle.min_;

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
