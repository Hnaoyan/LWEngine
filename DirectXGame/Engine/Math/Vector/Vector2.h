#pragma once
#include <cmath>
#include "Engine/Math/Matrix/Matrix3x3.h"

class Vector2 final
{
public:

	float x;
	float y;
	//Vector2(float x_, float y_) : x(x_), y(y_) {}

#pragma region 四則演算
	// 加算減算
	Vector2 operator+(const Vector2& Other) const {
		return Vector2{ this->x + Other.x, this->y + Other.y };
	}
	Vector2 operator-(const Vector2& Other) const {
		return Vector2{ this->x - Other.x, this->y - Other.y };
	}

	Vector2& operator+=(const Vector2& Other) {
		this->x += Other.x;
		this->y += Other.y;
		return *this;
	}
	Vector2& operator-=(const Vector2& Other) {
		this->x -= Other.x;
		this->y -= Other.y;
		return *this;
	}

	// 乗算除算
	Vector2 operator*(const Vector2& Other) const {
		return Vector2{ this->x * Other.x, this->y * Other.y };
	}
	Vector2 operator/(const Vector2& Other) const {
		return Vector2{ this->x / Other.x, this->y / Other.y };
	}

	Vector2 operator*(const float& Scaler) const {
		return Vector2{ this->x * Scaler, this->y * Scaler };
	}
	Vector2 operator/(const float& Scaler) const {
		return Vector2{ this->x / Scaler, this->y / Scaler };
	}

	Vector2& operator*=(const float& Scaler) {
		this->x *= Scaler;
		this->y *= Scaler;
		return *this;
	}
	Vector2& operator/=(const float& Scaler) {
		this->x /= Scaler;
		this->y /= Scaler;
		return *this;
	}
	Vector2& operator=(const Vector2& Other) {
		this->x = Other.x;
		this->y = Other.y;
		return *this;
	}

	bool operator==(const Vector2& Other) {
		bool isX = this->x == Other.x;
		bool isY = this->y == Other.y;
		return isX && isY;
	}

	bool operator!=(const Vector2& Other) {
		bool isX = this->x == Other.x;
		bool isY = this->y == Other.y;
		return !isX && !isY;
	}

#pragma endregion

	/// <summary>
	/// 内積
	/// </summary>
	/// <param name="v1"></param>
	/// <param name="v2"></param>
	/// <returns></returns>
	inline static float Dot(const Vector2& v1, const Vector2& v2) {
		return float((v1.x * v2.x) + (v1.y * v2.y));
	}

	/// <summary>
	/// 外積
	/// </summary>
	/// <param name="v1"></param>
	/// <param name="v2"></param>
	/// <returns></returns>
	inline static float Cross(const Vector2& v1, const Vector2& v2) {
		return float(v1.x * v2.y - v1.y * v2.x);
	}

	inline static float Length(const Vector2& v1) {
		return float(std::sqrtf(std::powf(v1.x, 2) + std::powf(v1.y, 2)));
	}

	inline static float Distance(const Vector2& v1, const Vector2& v2) {
		Vector2 distance = v1 - v2;
		return float(std::sqrtf(std::powf(distance.x, 2) + std::powf(distance.y, 2)));
	}

	inline static Vector2 Normalize(const Vector2& v) {
		Vector2 result{};
		float length = Length(v);

		if (length != 0) {
			result.x = v.x / length;
			result.y = v.y / length;
		}
		return Vector2(result);
	}
	inline static Vector2 Lerp(const Vector2& start, const Vector2& end, float t) {
		Vector2 result{};

		result.x = (1.0f - t) * start.x + t * end.x;
		result.y = (1.0f - t) * start.y + t * end.y;

		return Vector2(result);
	}

	inline static Vector2 MakeRotateVector(const Vector2& vector,float radian) {
		// 角度による回転行列
		Matrix3x3 rotateMatrix = Matrix3x3::MakeRotateMatrix(radian);
		// 元のベクトルを回転させたもの
		return Vector2(Matrix3x3::Transform(vector, rotateMatrix));

	}
};
