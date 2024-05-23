#pragma once

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

};
