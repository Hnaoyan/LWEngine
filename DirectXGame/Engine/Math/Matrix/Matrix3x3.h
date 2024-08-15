#pragma once
#include <cassert>
#include "../Vector/Vector2.h"

class Matrix3x3 final
{
public:
	float m[3][3];

public:
	inline static Matrix3x3 MakeRotateMatrix(float theta = 0) {
		Matrix3x3 MakeRotateMatrix;
		MakeRotateMatrix.m[0][0] = cos(theta);
		MakeRotateMatrix.m[0][1] = sin(theta);
		MakeRotateMatrix.m[0][2] = 0;

		MakeRotateMatrix.m[1][0] = -sin(theta);
		MakeRotateMatrix.m[1][1] = cos(theta);
		MakeRotateMatrix.m[1][2] = 0;

		MakeRotateMatrix.m[2][0] = 0;
		MakeRotateMatrix.m[2][1] = 0;
		MakeRotateMatrix.m[2][2] = 1;

		return MakeRotateMatrix;
	}

	//
	inline static Matrix3x3 MakeTranslateMatrix(const Vector2& translate) {
		Matrix3x3 MakeTranslateMatrix;
		MakeTranslateMatrix.m[0][0] = 1, MakeTranslateMatrix.m[0][1] = 0, MakeTranslateMatrix.m[0][2] = 0;
		MakeTranslateMatrix.m[1][0] = 0, MakeTranslateMatrix.m[1][1] = 1, MakeTranslateMatrix.m[1][2] = 0;
		MakeTranslateMatrix.m[2][0] = translate.x,
			MakeTranslateMatrix.m[2][1] = translate.y,
			MakeTranslateMatrix.m[2][2] = 1;
		return MakeTranslateMatrix;

	}

	//行列の積
	inline static Matrix3x3 Multiply(const Matrix3x3& matrix1, const Matrix3x3& matrix2) {
		Matrix3x3 Multiply;
		Multiply.m[0][0] = matrix1.m[0][0] * matrix2.m[0][0];
		Multiply.m[0][1] = matrix1.m[0][1] * matrix2.m[0][0];
		Multiply.m[0][2] = matrix1.m[0][2] * matrix2.m[0][2];
		Multiply.m[1][0] = matrix1.m[1][0] * matrix2.m[1][1];
		Multiply.m[1][1] = matrix1.m[1][1] * matrix2.m[1][1];
		Multiply.m[1][2] = matrix1.m[1][2] * matrix2.m[1][2];
		Multiply.m[2][0] = matrix1.m[2][0] * matrix2.m[2][0];
		Multiply.m[2][1] = matrix1.m[2][1] * matrix2.m[2][1];
		Multiply.m[2][2] = matrix1.m[2][2] * matrix2.m[2][2];

		return Multiply;

	}

	inline static Vector2 Transform(const Vector2& vector, const Matrix3x3& matrix) {
		Vector2 result;
		result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + 1.0f * matrix.m[2][0];
		result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + 1.0f * matrix.m[2][1];
		float w = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + 1.0f * matrix.m[2][2];
		assert(w != 0.0f);
		result.x /= w;
		result.y /= w;
		return result;
	}
};
