#pragma once
#include "Vector/Vector3.h"

class Quaternion {
public:
	float x, y, z, w;

public: // 基本的な計算
	// 積
	static Quaternion Multiply(const Quaternion& lhs, const Quaternion& rhs);
	// 単位
	static Quaternion IdentityQuaternion();
	// 共役
	static Quaternion Conjugate(const Quaternion& quaternion);
	// norm
	static float Norm(const Quaternion& quaternion);
	// 正規化
	static Quaternion Normalize(const Quaternion& quaternion);
	// 逆
	static Quaternion Inverse(const Quaternion& quaternion);
	// 内積
	static float Dot(const Quaternion& q0, const Quaternion& q1);
	// 球面線形補間
	static Quaternion Slerp(const Quaternion& q0, const Quaternion& q1, float t);
	// スカラー
	static Quaternion Scaler(const Quaternion& q, float scaler);
	// 加算
	static Quaternion Add(const Quaternion& q0, const Quaternion& q1);

public:
	/// <summary>
	/// 任意軸回転を表すクォータニオン
	/// </summary>
	/// <param name="axis"></param>
	/// <param name="angle"></param>
	/// <returns></returns>
	static Quaternion MakeRotateAxisAngleQuaternion(const Vector3& axis, float angle);

};
