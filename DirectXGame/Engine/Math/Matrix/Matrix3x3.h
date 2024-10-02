#pragma once

class Vector2;

class Matrix3x3 final
{
public:
	float m[3][3];

public:
	static Matrix3x3 MakeRotateMatrix(const float& theta = 0);

	//
	static Matrix3x3 MakeTranslateMatrix(const Vector2& translate);

	//行列の積
	static Matrix3x3 Multiply(const Matrix3x3& matrix1, const Matrix3x3& matrix2);

	static Vector2 Transform(const Vector2& vector, const Matrix3x3& matrix);
};
