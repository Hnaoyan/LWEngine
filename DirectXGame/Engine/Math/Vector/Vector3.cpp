#include "Vector3.h"
#include "../Quaternion.h"

Vector3 Vector3::RotateVector(const Vector3& vector, const Quaternion& quaternion)
{
	Vector3 result{};
	Quaternion r = { vector.x,vector.y,vector.z,0 };
	Quaternion normalize = Quaternion::Normalize(quaternion);
	Quaternion rotateQuat = Quaternion::Multiply(Quaternion::Multiply(normalize, r), Quaternion::Conjugate(normalize));
	result = { rotateQuat.x,rotateQuat.y,rotateQuat.z };

	return result;
}

Vector3 Vector3::RotateVector(const Vector3& from, const Vector3& to)
{
    // 正規化されたベクトルを取得
    Vector3 normFrom = Vector3::Normalize(from);
    Vector3 normTo = Vector3::Normalize(to);

    // 回転軸を計算 (from と to が平行の場合、外積はゼロになるため回転不要)
    Vector3 axis = Vector3::Normalize(Vector3::Cross(normFrom, normTo));
    if (Vector3::Length(axis) < 1e-6) {
        // 回転軸がほぼゼロの場合（平行または逆平行）、そのまま返す
        return normTo;
    }

    // 回転角を計算
    float angle = std::acos(Vector3::Dot(normFrom, normTo));
    float cosA = std::cos(angle);
    float sinA = std::sin(angle);

    // ロドリゲスの公式を使った回転計算
    Vector3 rotated =
        normFrom * cosA +
        axis * sinA * Vector3::Cross(axis, normFrom) +
        axis * (1 - cosA) * Vector3::Dot(axis, normFrom);

    return rotated;
}