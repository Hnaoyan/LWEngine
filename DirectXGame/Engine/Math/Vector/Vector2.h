#pragma once

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

	inline static float Dot(const Vector2& v1, const Vector2& v2) {
		return float((v1.x * v2.x) + (v1.y * v2.y));
	}

	inline static float Length(const Vector2& v1) {
		return float(sqrt(powf(v1.x, 2) + powf(v1.y, 2)));
	}

	inline static float Distance(const Vector2& v1, const Vector2& v2) {
		Vector2 distance = v1 - v2;
		return float(sqrtf(powf(distance.x, 2) + powf(distance.y, 2)));
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
};
