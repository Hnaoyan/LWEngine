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

float Quaternion::Dot(const Quaternion& q0, const Quaternion& q1)
{
    return ((q0.x * q1.x) + (q0.y * q1.y) + (q0.z * q1.z) + (q0.w * q1.w));
}

Quaternion Quaternion::Slerp(const Quaternion& q0, const Quaternion& q1, const float& t)
{
    Quaternion quaternion0 = q0;
    float dot = Dot(q0, q1);

    if (dot < 0) {
        quaternion0 = { -q0.x, -q0.y, -q0.z, -q0.w };
        dot = -dot;
    }

    if (dot >= 1.0f - 0.0005f) {
        return Quaternion{
            (1.0f - t) * quaternion0.x + t * q1.x,
            (1.0f - t) * quaternion0.y + t * q1.y,
            (1.0f - t) * quaternion0.z + t * q1.z,
            (1.0f - t) * quaternion0.w + t * q1.w
        };
    }

    float theta = std::acos(dot);

    //
    float scale0 = std::sin((1.0f - t) * theta) / std::sin(theta);
    float scale1 = std::sin(t * theta) / std::sin(theta);

    return Quaternion{
        scale0 * quaternion0.x + scale1 * q1.x,
        scale0 * quaternion0.y + scale1 * q1.y,
        scale0 * quaternion0.z + scale1 * q1.z,
        scale0 * quaternion0.w + scale1 * q1.w
    };
}

Quaternion Quaternion::Scaler(const Quaternion& q, const float& scaler)
{
    return Quaternion(q.x * scaler, q.y * scaler, q.z * scaler, q.w * scaler);
}

Quaternion Quaternion::Add(const Quaternion& q0, const Quaternion& q1)
{
    return Quaternion(q0.x + q1.x, q0.y + q1.y, q0.z + q1.z, q0.w + q1.w);
}

Quaternion Quaternion::MakeRotateAxisAngleQuaternion(const Vector3& axis, const float& angle)
{
    Quaternion result = {};
    result.w = std::cosf(angle / 2.0f);
    result.x = std::sinf(angle / 2.0f) * axis.x;
    result.y = std::sinf(angle / 2.0f) * axis.y;
    result.z = std::sinf(angle / 2.0f) * axis.z;
    return result;
}
