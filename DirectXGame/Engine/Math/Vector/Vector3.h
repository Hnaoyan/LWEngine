#pragma once
#include <cmath>

class Vector3
{
public:
	float x;
	float y;
	float z;

	//Vector3(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}

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

	//bool operator==(const Vector3& Other) const{
	//	return 
	//}

#pragma endregion

};

