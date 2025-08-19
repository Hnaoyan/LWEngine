#include "PenetrationResolver.h"

Vector3 PenetrationResolver::ExtrusionCalculation(AABB from, AABB to)
{
    Vector3 max = from.max_ - to.max_;
    Vector3 min = from.min_ - to.min_;

    float overlapX1 = from.max_.x - to.min_.x;  // 右から
    float overlapX2 = from.min_.x - to.max_.x;  // 左から
    float overlapX = (overlapX1 < overlapX2) ? overlapX1 : -overlapX2;

    float overlapY1 = from.max_.y - to.min_.y;  // 上から
    float overlapY2 = from.min_.y - to.max_.y;  // 下から
    float overlapY = (overlapY1 < overlapY2) ? overlapY1 : -overlapY2;

    float overlapZ1 = from.max_.z - to.min_.z;  // 前から
    float overlapZ2 = from.min_.z - to.max_.z;  // 後から
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

    return Vector3();
}
