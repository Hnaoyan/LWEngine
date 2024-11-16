#include "Quaternion.h"
#include "Engine/LwLib/LwEngineLib.h"
#include <cmath>
#include <numbers>
#include <algorithm>

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
    Quaternion result{};
    result.w = std::cosf(angle / 2.0f);
    float sinf = std::sinf(angle / 2.0f);
    result.x = sinf * axis.x;
    result.y = sinf * axis.y;
    result.z = sinf * axis.z;
    return result;
}

Quaternion Quaternion::MakeRotateToDirect(const Vector3& direct, const Vector3& axis)
{
    //// 角度計算
    //float dot = Vector3::Dot(Vector3::Normalize(axis), Vector3::Normalize(direct));
    //dot = std::clamp(dot, -1.0f, 1.0f);
    //float angle = std::acosf(dot);
    //Vector3 rotAxis = Vector3::Cross(Vector3::Normalize(axis), Vector3::Normalize(direct));
    //return Quaternion::MakeRotateAxisAngleQuaternion(rotAxis, angle);


    // 角度計算: 内積を使って角度を計算
    float cosAngle = Vector3::Dot(Vector3::Normalize(axis), Vector3::Normalize(direct));
    cosAngle = std::clamp(cosAngle, -1.0f, 1.0f);  // 数値誤差で1.0fを超えないようにクランプ
    float angle = std::acosf(cosAngle);
    Vector3 rotationAxis{};
    if (std::abs(cosAngle - 1.0f) < 0.001f) {
        // ベクトルが同じ方向
        rotationAxis = Vector3(1.0f, 0.0f, 0.0f); 
    }
    else if (std::abs(cosAngle + 1.0f) < 0.001f) {
        // ベクトルが逆方向
        rotationAxis = Vector3(1.0f, 0.0f, 0.0f);  // 垂直な軸
    }
    else {
        rotationAxis = Vector3::Cross(Vector3::Normalize(axis), Vector3::Normalize(direct));  // 外積で回転軸を求める
        rotationAxis.Normalize();  // 正規化
    }
    return Quaternion(Quaternion::MakeRotateAxisAngleQuaternion(rotationAxis, angle));
}

Quaternion Quaternion::MakeRotateDirect(const Vector3& direct)
{
    Vector3 normDirect = Vector3::Normalize(direct);
    //Vector3 angle{};
    //angle.x = std::acosf(Vector3::Dot(Vector3::Forward(), normDirect));
    //angle.y = std::acosf(Vector3::Dot(Vector3::Forward(), normDirect));
    //Quaternion qx = Quaternion::MakeRotateAxisAngleQuaternion(Vector3::Forward(), angle.x);
    //Quaternion qy = Quaternion::MakeRotateAxisAngleQuaternion(Vector3::Forward(), angle.y);
    //angle.z = std::acosf(Vector3::Dot(Vector3::Forward(), normDirect));
    //Quaternion qz = Quaternion::MakeRotateAxisAngleQuaternion(Vector3::Forward(), angle.z);
    //float angleZ = std::atan2(normDirect.y, normDirect.x);
    //Quaternion qz = Quaternion::MakeRotateAxisAngleQuaternion(Vector3::Forward(), angleZ);

    float angleX = std::atan2(normDirect.y, std::sqrt(normDirect.x * normDirect.x + normDirect.z * normDirect.z));
    float angleY = std::atan2(normDirect.x, normDirect.z);
    Quaternion qx = Quaternion::MakeRotateAxisAngleQuaternion(Vector3::Right(), -angleX);
    Quaternion qy = Quaternion::MakeRotateAxisAngleQuaternion(Vector3::Up(), angleY);


    return Quaternion(qy * qx); 
}

Quaternion Quaternion::DirectionToDirection(const Vector3& from, const Vector3& to)
{
    Vector3 normFrom, normTo;
    normFrom = Vector3::Normalize(from);
    normTo = Vector3::Normalize(to);

    Vector3 cross = Vector3::Cross(normFrom, normTo);
    float cosAngle = Vector3::Dot(normFrom, normTo);
    float angle = std::acosf(cosAngle);
    float sinAngle = std::sinf(angle / 2.0f);

    return Quaternion(cross.x * sinAngle, cross.y * sinAngle, cross.z * sinAngle);
}
