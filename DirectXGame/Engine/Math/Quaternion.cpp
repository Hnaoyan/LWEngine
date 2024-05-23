#include "Quaternion.h"
#include <cmath>
#include <numbers>

Quaternion Quaternion::Multiply(const Quaternion& lhs, const Quaternion& rhs)
{
    Quaternion result = {};
    result.w = lhs.w * rhs.w - lhs.x * rhs.x - lhs.y * rhs.y - lhs.z * rhs.z;
    result.x = lhs.y * rhs.z - lhs.z * rhs.y + rhs.w * lhs.x + lhs.w * rhs.x;
    result.y = lhs.z * rhs.x - lhs.x * rhs.z + rhs.w * lhs.y + lhs.w * rhs.y;
    result.z = lhs.x * rhs.y - rhs.x * lhs.y + rhs.w * lhs.z + lhs.w * rhs.z;
    return result;
}

Quaternion Quaternion::IdentityQuaternion()
{
    return Quaternion(0.0f, 0.0f, 0.0f, 1.0f);
}

Quaternion Quaternion::Conjugate(const Quaternion& quaternion)
{
    return Quaternion(-quaternion.x, -quaternion.y, -quaternion.z, quaternion.w);
}

float Quaternion::Norm(const Quaternion& quaternion)
{
    float norm = 0;
    norm = std::sqrtf(std::powf(quaternion.w, 2) + std::powf(quaternion.x, 2) + std::powf(quaternion.y, 2) + std::powf(quaternion.z, 2));
    return norm;
}

Quaternion Quaternion::Normalize(const Quaternion& quaternion)
{
    Quaternion result = {};
    float length = Norm(quaternion);
    if (length != 0) {
        result.w = quaternion.w / length;
        result.x = quaternion.x / length;
        result.y = quaternion.y / length;
        result.z = quaternion.z / length;
    }
    return result;
}

Quaternion Quaternion::Inverse(const Quaternion& quaternion)
{
    Quaternion result = Conjugate(quaternion);
    float length = std::powf(Norm(quaternion), 2);

    if (length != 0) {
        result.x /= length;
        result.y /= length;
        result.z /= length;
        result.w /= length;
    }

    //Quaternion
    return result;
}
