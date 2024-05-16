#pragma once

class Vector2 {
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
#pragma endregion

};
