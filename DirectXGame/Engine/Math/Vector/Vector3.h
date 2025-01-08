#pragma once
#include <cmath>

class Quaternion;

class Vector3 final
{
public:
	float x;
	float y;
	float z;
#pragma region 四則演算

	Vector3 operator+(const Vector3& Other) const {
		return Vector3{ this->x + Other.x, this->y + Other.y, this->z + Other.z };
	}
	Vector3 operator-(const Vector3& Other) const {
		return Vector3{ this->x - Other.x, this->y - Other.y, this->z - Other.z };
	}
	Vector3 operator*(const Vector3& Other) const {
		return Vector3{ this->x * Other.x, this->y * Other.y, this->z * Other.z };
	}
	Vector3 operator/(const Vector3& Other) const {
		return Vector3{ this->x / Other.x, this->y / Other.y, this->z / Other.z };
	}

	Vector3 operator*(const float& Scaler) const {
		return Vector3{ this->x * Scaler, this->y * Scaler, this->z * Scaler };
	}
	Vector3 operator/(const float& Scaler) const {
		return Vector3{ this->x / Scaler, this->y / Scaler, this->z / Scaler };
	}
public: // 複合演算子
	// +=
	Vector3& operator+=(const Vector3& Other) {
		this->x += Other.x;
		this->y += Other.y;
		this->z += Other.z;
		return *this;
	}
	// -=
	Vector3& operator-=(const Vector3& Other) {
		this->x -= Other.x;
		this->y -= Other.y;
		this->z -= Other.z;
		return *this;
	}
	// *=
	Vector3& operator*=(const float& Scaler) {
		this->x *= Scaler;
		this->y *= Scaler;
		this->z *= Scaler;
		return *this;
	}
	// /=
	Vector3& operator/=(const float& Scaler) {
		this->x /= Scaler;
		this->y /= Scaler;
		this->z /= Scaler;
		return *this;
	}

	Vector3& operator=(const Vector3& Other) {
		this->x = Other.x;
		this->y = Other.y;
		this->z = Other.z;
		return *this;
	}

	bool operator==(const Vector3& Other) {
		bool isX = this->x == Other.x;
		bool isY = this->y == Other.y;
		bool isZ = this->z == Other.z;
		return isX && isY && isZ;
	}

	bool operator!=(const Vector3& Other) {
		bool isX = this->x == Other.x;
		bool isY = this->y == Other.y;
		bool isZ = this->z == Other.z;
		return !isX && !isY && !isZ;
	}

	inline static Vector3 Scaler(const Vector3& me, float scaler) {
		return Vector3(me.x * scaler, me.y * scaler, me.z * scaler);
	}

	inline static Vector3 Reflect(const Vector3& input, const Vector3& normal) {
		return Vector3(input - (normal * (Vector3::Dot(input, normal) * 2.0f)));
	}

#pragma endregion

#pragma region デフォルト関数
	inline static Vector3 Zero() { return Vector3(0.0f, 0.0f, 0.0f); }
	inline static Vector3 Right() { return Vector3(1.0f, 0.0f, 0.0f); }
	inline static Vector3 Left() { return Vector3(Right() * -1.0f); }
	inline static Vector3 Up() { return Vector3(0.0f, 1.0f, 0.0f); }
	inline static Vector3 Down() { return Vector3(Up() * -1.0f); }
	inline static Vector3 Forward() { return Vector3(0.0f, 0.0f, 1.0f); }
	inline static Vector3 Backward() { return Vector3(Forward() * -1.0f); }
	inline static Vector3 DefaultOnes() { return Vector3(1.0f, 1.0f, 1.0f); }
#pragma endregion

	/// <summary>
	/// 内積
	/// </summary>
	/// <param name="v1"></param>
	/// <param name="v2"></param>
	/// <returns></returns>
	inline static float Dot(const Vector3& v1, const Vector3& v2) {
		return float((v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z));
	}

	/// <summary>
	/// 外積
	/// </summary>
	/// <param name="v1"></param>
	/// <param name="v2"></param>
	/// <returns></returns>
	inline static Vector3 Cross(const Vector3& v1, const Vector3& v2) {
		return Vector3((v1.y * v2.z - v1.z * v2.y), (v1.z * v2.x - v1.x * v2.z), (v1.x * v2.y - v1.y * v2.x));
	}

	/// <summary>
	/// 長さ
	/// </summary>
	/// <param name="v1"></param>
	/// <returns></returns>
	inline static float Length(const Vector3& v1) {
		return float(std::sqrtf((v1.x * v1.x) + (v1.y * v1.y) + (v1.z * v1.z)));
	}

	/// <summary>
	/// 距離
	/// </summary>
	/// <param name="v1"></param>
	/// <param name="v2"></param>
	/// <returns></returns>
	inline static float Distance(const Vector3& v1, const Vector3& v2) {
		Vector3 distance = v1 - v2;
		return float(std::sqrtf(std::powf(distance.x, 2) + std::powf(distance.y, 2) + std::powf(distance.z, 2)));
	}

	/// <summary>
	/// 正規化
	/// </summary>
	/// <param name="v"></param>
	/// <returns></returns>
	inline static Vector3 Normalize(const Vector3& v) {
		Vector3 result{};
		float length = Length(v);

		if (length != 0) {
			result.x = v.x / length;
			result.y = v.y / length;
			result.z = v.z / length;
		}
		return Vector3(result);
	}

	Vector3 Normalize() {
		Vector3 result{};
		float length = Length(*this);
		if (length != 0) {
			result.x = (*this).x / length;
			result.y = (*this).y / length;
			result.z = (*this).z / length;
		}
		return Vector3(result);
	}

	/// <summary>
	/// 線形補間
	/// </summary>
	/// <param name="start"></param>
	/// <param name="end"></param>
	/// <param name="t"></param>
	/// <returns></returns>
	inline static Vector3 Lerp(const Vector3& start, const Vector3& end, const float& t) {
		Vector3 result{};

		result.x = (1.0f - t) * start.x + t * end.x;
		result.y = (1.0f - t) * start.y + t * end.y;
		result.z = (1.0f - t) * start.z + t * end.z;

		return Vector3(result);
	}

	/// <summary>
	/// クォータニオンでベクトルを回転させる
	/// </summary>
	/// <param name="vector"></param>
	/// <param name="quaternion"></param>
	/// <returns></returns>
	static Vector3 RotateVector(const Vector3& vector, const Quaternion& quaternion);

	static Vector3 RotateVector(const Vector3& from, const Vector3& to);
};

