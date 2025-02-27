#include "Matrix4x4.h"
#include "../MathLib.h"

#include <cmath>
#include <numbers>
#include <cassert>

Matrix4x4 Matrix4x4::MakeInverse(const Matrix4x4& m)
{
	Matrix4x4 result = {};
	float det = 0;
	det += (m.m[0][0] * m.m[1][1] * m.m[2][2] * m.m[3][3]);
	det += (m.m[0][0] * m.m[1][2] * m.m[2][3] * m.m[3][1]);
	det += (m.m[0][0] * m.m[1][3] * m.m[2][1] * m.m[3][2]);

	det -= (m.m[0][0] * m.m[1][3] * m.m[2][2] * m.m[3][1]);
	det -= (m.m[0][0] * m.m[1][2] * m.m[2][1] * m.m[3][3]);
	det -= (m.m[0][0] * m.m[1][1] * m.m[2][3] * m.m[3][2]);

	det -= (m.m[0][1] * m.m[1][0] * m.m[2][2] * m.m[3][3]);
	det -= (m.m[0][2] * m.m[1][0] * m.m[2][3] * m.m[3][1]);
	det -= (m.m[0][3] * m.m[1][0] * m.m[2][1] * m.m[3][2]);

	det += (m.m[0][3] * m.m[1][0] * m.m[2][2] * m.m[3][1]);
	det += (m.m[0][2] * m.m[1][0] * m.m[2][1] * m.m[3][3]);
	det += (m.m[0][1] * m.m[1][0] * m.m[2][3] * m.m[3][2]);

	det += (m.m[0][1] * m.m[1][2] * m.m[2][0] * m.m[3][3]);
	det += (m.m[0][2] * m.m[1][3] * m.m[2][0] * m.m[3][1]);
	det += (m.m[0][3] * m.m[1][1] * m.m[2][0] * m.m[3][2]);

	det -= (m.m[0][3] * m.m[1][2] * m.m[2][0] * m.m[3][1]);
	det -= (m.m[0][2] * m.m[1][1] * m.m[2][0] * m.m[3][3]);
	det -= (m.m[0][1] * m.m[1][3] * m.m[2][0] * m.m[3][2]);

	det -= (m.m[0][1] * m.m[1][2] * m.m[2][3] * m.m[3][0]);
	det -= (m.m[0][2] * m.m[1][3] * m.m[2][1] * m.m[3][0]);
	det -= (m.m[0][3] * m.m[1][1] * m.m[2][2] * m.m[3][0]);

	det += (m.m[0][3] * m.m[1][2] * m.m[2][1] * m.m[3][0]);
	det += (m.m[0][2] * m.m[1][1] * m.m[2][3] * m.m[3][0]);
	det += (m.m[0][1] * m.m[1][3] * m.m[2][2] * m.m[3][0]);

	if (det != 0) {
		{
			result.m[0][0] =
				(1.0f / det) *
				(m.m[1][1] * m.m[2][2] * m.m[3][3] + m.m[1][2] * m.m[2][3] * m.m[3][1] +
					m.m[1][3] * m.m[2][1] * m.m[3][2] - m.m[1][3] * m.m[2][2] * m.m[3][1] -
					m.m[1][2] * m.m[2][1] * m.m[3][3] - m.m[1][1] * m.m[2][3] * m.m[3][2]);
			result.m[0][1] =
				(1.0f / det) *
				(-m.m[0][1] * m.m[2][2] * m.m[3][3] - m.m[0][2] * m.m[2][3] * m.m[3][1] -
					m.m[0][3] * m.m[2][1] * m.m[3][2] + m.m[0][3] * m.m[2][2] * m.m[3][1] +
					m.m[0][2] * m.m[2][1] * m.m[3][3] + m.m[0][1] * m.m[2][3] * m.m[3][2]);
			result.m[0][2] =
				(1.0f / det) *
				(+m.m[0][1] * m.m[1][2] * m.m[3][3] + m.m[0][2] * m.m[1][3] * m.m[3][1] +
					m.m[0][3] * m.m[1][1] * m.m[3][2] - m.m[0][3] * m.m[1][2] * m.m[3][1] -
					m.m[0][2] * m.m[1][1] * m.m[3][3] - m.m[0][1] * m.m[1][3] * m.m[3][2]);
			result.m[0][3] =
				(1.0f / det) *
				(-m.m[0][1] * m.m[1][2] * m.m[2][3] - m.m[0][2] * m.m[1][3] * m.m[2][1] -
					m.m[0][3] * m.m[1][1] * m.m[2][2] + m.m[0][3] * m.m[1][2] * m.m[2][1] +
					m.m[0][2] * m.m[1][1] * m.m[2][3] + m.m[0][1] * m.m[1][3] * m.m[2][2]);
		}

		{
			result.m[1][0] =
				(1.0f / det) *
				(-m.m[1][0] * m.m[2][2] * m.m[3][3] - m.m[1][2] * m.m[2][3] * m.m[3][0] -
					m.m[1][3] * m.m[2][0] * m.m[3][2] + m.m[1][3] * m.m[2][2] * m.m[3][0] +
					m.m[1][2] * m.m[2][0] * m.m[3][3] + m.m[1][0] * m.m[2][3] * m.m[3][2]);
			result.m[1][1] =
				(1.0f / det) *
				(m.m[0][0] * m.m[2][2] * m.m[3][3] + m.m[0][2] * m.m[2][3] * m.m[3][0] +
					m.m[0][3] * m.m[2][0] * m.m[3][2] - m.m[0][3] * m.m[2][2] * m.m[3][0] -
					m.m[0][2] * m.m[2][0] * m.m[3][3] - m.m[0][0] * m.m[2][3] * m.m[3][2]);
			result.m[1][2] =
				(1.0f / det) *
				(-m.m[0][0] * m.m[1][2] * m.m[3][3] - m.m[0][2] * m.m[1][3] * m.m[3][0] -
					m.m[0][3] * m.m[1][0] * m.m[3][2] + m.m[0][3] * m.m[1][2] * m.m[3][0] +
					m.m[0][2] * m.m[1][0] * m.m[3][3] + m.m[0][0] * m.m[1][3] * m.m[3][2]);
			result.m[1][3] =
				(1.0f / det) *
				(m.m[0][0] * m.m[1][2] * m.m[2][3] + m.m[0][2] * m.m[1][3] * m.m[2][0] +
					m.m[0][3] * m.m[1][0] * m.m[2][2] - m.m[0][3] * m.m[1][2] * m.m[2][0] -
					m.m[0][2] * m.m[1][0] * m.m[2][3] - m.m[0][0] * m.m[1][3] * m.m[2][2]);
		}

		{
			result.m[2][0] =
				(1.0f / det) *
				(m.m[1][0] * m.m[2][1] * m.m[3][3] + m.m[1][1] * m.m[2][3] * m.m[3][0] +
					m.m[1][3] * m.m[2][0] * m.m[3][1] - m.m[1][3] * m.m[2][1] * m.m[3][0] -
					m.m[1][1] * m.m[2][0] * m.m[3][3] - m.m[1][0] * m.m[2][3] * m.m[3][1]);
			result.m[2][1] =
				(1.0f / det) *
				(-m.m[0][0] * m.m[2][1] * m.m[3][3] - m.m[0][1] * m.m[2][3] * m.m[3][0] -
					m.m[0][3] * m.m[2][0] * m.m[3][1] + m.m[0][3] * m.m[2][1] * m.m[3][0] +
					m.m[0][1] * m.m[2][0] * m.m[3][3] + m.m[0][0] * m.m[2][3] * m.m[3][1]);
			result.m[2][2] =
				(1.0f / det) *
				(m.m[0][0] * m.m[1][1] * m.m[3][3] + m.m[0][1] * m.m[1][3] * m.m[3][0] +
					m.m[0][3] * m.m[1][0] * m.m[3][1] - m.m[0][3] * m.m[1][1] * m.m[3][0] -
					m.m[0][1] * m.m[1][0] * m.m[3][3] - m.m[0][0] * m.m[1][3] * m.m[3][1]);
			result.m[2][3] =
				(1.0f / det) *
				(-m.m[0][0] * m.m[1][1] * m.m[2][3] - m.m[0][1] * m.m[1][3] * m.m[2][0] -
					m.m[0][3] * m.m[1][0] * m.m[2][1] + m.m[0][3] * m.m[1][1] * m.m[2][0] +
					m.m[0][1] * m.m[1][0] * m.m[2][3] + m.m[0][0] * m.m[1][3] * m.m[2][1]);
		}

		result.m[3][0] =
			(1.0f / det) * (-m.m[1][0] * m.m[2][1] * m.m[3][2] - m.m[1][1] * m.m[2][2] * m.m[3][0] -
				m.m[1][2] * m.m[2][0] * m.m[3][1] + m.m[1][2] * m.m[2][1] * m.m[3][0] +
				m.m[1][1] * m.m[2][0] * m.m[3][2] + m.m[1][0] * m.m[2][2] * m.m[3][1]);
		result.m[3][1] =
			(1.0f / det) * (m.m[0][0] * m.m[2][1] * m.m[3][2] + m.m[0][1] * m.m[2][2] * m.m[3][0] +
				m.m[0][2] * m.m[2][0] * m.m[3][1] - m.m[0][2] * m.m[2][1] * m.m[3][0] -
				m.m[0][1] * m.m[2][0] * m.m[3][2] - m.m[0][0] * m.m[2][2] * m.m[3][1]);

		result.m[3][2] =
			(1.0f / det) * (-m.m[0][0] * m.m[1][1] * m.m[3][2] - m.m[0][1] * m.m[1][2] * m.m[3][0] -
				m.m[0][2] * m.m[1][0] * m.m[3][1] + m.m[0][2] * m.m[1][1] * m.m[3][0] +
				m.m[0][1] * m.m[1][0] * m.m[3][2] + m.m[0][0] * m.m[1][2] * m.m[3][1]);
		result.m[3][3] =
			(1.0f / det) * (m.m[0][0] * m.m[1][1] * m.m[2][2] + m.m[0][1] * m.m[1][2] * m.m[2][0] +
				m.m[0][2] * m.m[1][0] * m.m[2][1] - m.m[0][2] * m.m[1][1] * m.m[2][0] -
				m.m[0][1] * m.m[1][0] * m.m[2][2] - m.m[0][0] * m.m[1][2] * m.m[2][1]);
	}
	return Matrix4x4(result);
//Matrix4x4 result;
//float resultDet =
//(m.m[0][0] * m.m[1][1] * m.m[2][2] * m.m[3][3])
//+ (m.m[0][0] * m.m[1][2] * m.m[2][3] * m.m[3][1])
//+ (m.m[0][0] * m.m[1][3] * m.m[2][1] * m.m[3][2]) //3行目
//- (m.m[0][0] * m.m[1][3] * m.m[2][2] * m.m[3][1])
//- (m.m[0][0] * m.m[1][2] * m.m[2][1] * m.m[3][3])
//- (m.m[0][0] * m.m[1][1] * m.m[2][3] * m.m[3][2]) //6行目
//- (m.m[0][1] * m.m[1][0] * m.m[2][2] * m.m[3][3])
//- (m.m[0][2] * m.m[1][0] * m.m[2][3] * m.m[3][1])
//- (m.m[0][3] * m.m[1][0] * m.m[2][1] * m.m[3][2]) //9行目
//+ (m.m[0][3] * m.m[1][0] * m.m[2][2] * m.m[3][1])
//+ (m.m[0][2] * m.m[1][0] * m.m[2][1] * m.m[3][3])
//+ (m.m[0][1] * m.m[1][0] * m.m[2][3] * m.m[3][2]) //12行目
//+ (m.m[0][1] * m.m[1][2] * m.m[2][0] * m.m[3][3])
//+ (m.m[0][2] * m.m[1][3] * m.m[2][0] * m.m[3][1])
//+ (m.m[0][3] * m.m[1][1] * m.m[2][0] * m.m[3][2]) //15行目
//- (m.m[0][3] * m.m[1][2] * m.m[2][0] * m.m[3][1])
//- (m.m[0][2] * m.m[1][1] * m.m[2][0] * m.m[3][3])
//- (m.m[0][1] * m.m[1][3] * m.m[2][0] * m.m[3][2]) //18行目
//- (m.m[0][1] * m.m[1][2] * m.m[2][3] * m.m[3][0])
//- (m.m[0][2] * m.m[1][3] * m.m[2][1] * m.m[3][0])
//- (m.m[0][3] * m.m[1][1] * m.m[2][2] * m.m[3][0]) //21行目
//+ (m.m[0][3] * m.m[1][2] * m.m[2][1] * m.m[3][0])
//+ (m.m[0][2] * m.m[1][1] * m.m[2][3] * m.m[3][0])
//+ (m.m[0][1] * m.m[1][3] * m.m[2][2] * m.m[3][0]); //24行目;
//
//result.m[0][0] = (m.m[1][1] * m.m[2][2] * m.m[3][3] + m.m[1][2] * m.m[2][3] * m.m[3][1] + m.m[1][3] * m.m[2][1] * m.m[3][2]
//	- m.m[1][3] * m.m[2][2] * m.m[3][1] - m.m[1][2] * m.m[2][1] * m.m[3][3] - m.m[1][1] * m.m[2][3] * m.m[3][2]) / resultDet;
//result.m[0][1] = (-m.m[0][1] * m.m[2][2] * m.m[3][3] - m.m[0][2] * m.m[2][3] * m.m[3][1] - m.m[0][3] * m.m[2][1] * m.m[3][2]
//	+ m.m[0][3] * m.m[2][2] * m.m[3][1] + m.m[0][2] * m.m[2][1] * m.m[3][3] + m.m[0][1] * m.m[2][3] * m.m[3][2]) / resultDet;
//result.m[0][2] = (m.m[0][1] * m.m[1][2] * m.m[3][3] + m.m[0][2] * m.m[1][3] * m.m[3][1] + m.m[0][3] * m.m[1][1] * m.m[3][2]
//	- m.m[0][3] * m.m[1][2] * m.m[3][1] - m.m[0][2] * m.m[1][1] * m.m[3][3] - m.m[0][1] * m.m[1][3] * m.m[3][2]) / resultDet;
//result.m[0][3] = (-m.m[0][1] * m.m[1][2] * m.m[2][3] - m.m[0][2] * m.m[1][3] * m.m[2][1] - m.m[0][3] * m.m[1][1] * m.m[2][2]
//	+ m.m[0][3] * m.m[1][2] * m.m[2][1] + m.m[0][2] * m.m[1][1] * m.m[2][3] + m.m[0][1] * m.m[1][3] * m.m[2][2]) / resultDet;
//
//result.m[1][0] = (-m.m[1][0] * m.m[2][2] * m.m[3][3] - m.m[1][2] * m.m[2][3] * m.m[3][0] - m.m[1][3] * m.m[2][0] * m.m[3][2]
//	+ m.m[1][3] * m.m[2][2] * m.m[3][0] + m.m[1][2] * m.m[2][0] * m.m[3][3] + m.m[1][0] * m.m[2][3] * m.m[3][2]) / resultDet;
//result.m[1][1] = (m.m[0][0] * m.m[2][2] * m.m[3][3] + m.m[0][2] * m.m[2][3] * m.m[3][0] + m.m[0][3] * m.m[2][0] * m.m[3][2]
//	- m.m[0][3] * m.m[2][2] * m.m[3][0] - m.m[0][2] * m.m[2][0] * m.m[3][3] - m.m[0][0] * m.m[2][3] * m.m[3][2]) / resultDet;
//result.m[1][2] = (-m.m[0][0] * m.m[1][2] * m.m[3][3] - m.m[0][2] * m.m[1][3] * m.m[3][0] - m.m[0][3] * m.m[1][0] * m.m[3][2]
//	+ m.m[0][3] * m.m[1][2] * m.m[3][0] + m.m[0][2] * m.m[1][0] * m.m[3][3] + m.m[0][0] * m.m[1][3] * m.m[3][2]) / resultDet;
//result.m[1][3] = (m.m[0][0] * m.m[1][2] * m.m[2][3] + m.m[0][2] * m.m[1][3] * m.m[2][0] + m.m[0][3] * m.m[1][0] * m.m[2][2]
//	- m.m[0][3] * m.m[1][2] * m.m[2][0] - m.m[0][2] * m.m[1][0] * m.m[2][3] - m.m[0][0] * m.m[1][3] * m.m[2][2]) / resultDet;
//
//result.m[2][0] = (m.m[1][0] * m.m[2][1] * m.m[3][3] + m.m[1][1] * m.m[2][3] * m.m[3][0] + m.m[1][3] * m.m[2][0] * m.m[3][1]
//	- m.m[1][3] * m.m[2][1] * m.m[3][0] - m.m[1][1] * m.m[2][0] * m.m[3][3] - m.m[1][0] * m.m[2][3] * m.m[3][1]) / resultDet;
//result.m[2][1] = (-m.m[0][0] * m.m[2][1] * m.m[3][3] - m.m[0][1] * m.m[2][3] * m.m[3][0] - m.m[0][3] * m.m[2][0] * m.m[3][1]
//	+ m.m[0][3] * m.m[2][1] * m.m[3][0] + m.m[0][1] * m.m[2][0] * m.m[3][3] + m.m[0][0] * m.m[2][3] * m.m[3][1]) / resultDet;
//result.m[2][2] = (m.m[0][0] * m.m[1][1] * m.m[3][3] + m.m[0][1] * m.m[1][3] * m.m[3][0] + m.m[0][3] * m.m[1][0] * m.m[3][1]
//	- m.m[0][3] * m.m[1][1] * m.m[3][0] - m.m[0][1] * m.m[1][0] * m.m[3][3] - m.m[0][0] * m.m[1][3] * m.m[3][1]) / resultDet;
//result.m[2][3] = (-m.m[0][0] * m.m[1][1] * m.m[2][3] - m.m[0][1] * m.m[1][3] * m.m[2][0] - m.m[0][3] * m.m[1][0] * m.m[2][1]
//	+ m.m[0][3] * m.m[1][1] * m.m[2][0] + m.m[0][1] * m.m[1][0] * m.m[2][3] + m.m[0][0] * m.m[1][3] * m.m[2][1]) / resultDet;
//
//result.m[3][0] = (-m.m[1][0] * m.m[2][1] * m.m[3][2] - m.m[1][1] * m.m[2][2] * m.m[3][0] - m.m[1][2] * m.m[2][0] * m.m[3][1]
//	+ m.m[1][2] * m.m[2][1] * m.m[3][0] + m.m[1][1] * m.m[2][0] * m.m[3][2] + m.m[1][0] * m.m[2][2] * m.m[3][1]) / resultDet;
//result.m[3][1] = (m.m[0][0] * m.m[2][1] * m.m[3][2] + m.m[0][1] * m.m[2][2] * m.m[3][0] + m.m[0][2] * m.m[2][0] * m.m[3][1]
//	- m.m[0][2] * m.m[2][1] * m.m[3][0] - m.m[0][1] * m.m[2][0] * m.m[3][2] - m.m[0][0] * m.m[2][2] * m.m[3][1]) / resultDet;
//result.m[3][2] = (-m.m[0][0] * m.m[1][1] * m.m[3][2] - m.m[0][1] * m.m[1][2] * m.m[3][0] - m.m[0][2] * m.m[1][0] * m.m[3][1]
//	+ m.m[0][2] * m.m[1][1] * m.m[3][0] + m.m[0][1] * m.m[1][0] * m.m[3][2] + m.m[0][0] * m.m[1][2] * m.m[3][1]) / resultDet;
//result.m[3][3] = (m.m[0][0] * m.m[1][1] * m.m[2][2] + m.m[0][1] * m.m[1][2] * m.m[2][0] + m.m[0][2] * m.m[1][0] * m.m[2][1]
//	- m.m[0][2] * m.m[1][1] * m.m[2][0] - m.m[0][1] * m.m[1][0] * m.m[2][2] - m.m[0][0] * m.m[1][2] * m.m[2][1]) / resultDet;
//	return result;
}

Matrix4x4 Matrix4x4::MakeIdentity4x4()
{
	Matrix4x4 result = {};
	for (int y = 0; y < 4; y++) {
		for (int x = 0; x < 4; x++) {
			if (x == y) {
				result.m[y][x] = 1;
			}
		}
	}
	return result;
}

Matrix4x4 Matrix4x4::Multiply(const Matrix4x4& m1, const Matrix4x4& m2)
{
	Matrix4x4 result = {};
	for (int y = 0; y < 4; y++) {
		for (int x = 0; x < 4; x++) {
			result.m[y][x] = 0;
			for (int k = 0; k < 4; k++) {
				result.m[y][x] += m1.m[y][k] * m2.m[k][x];
			}
		}
	}
	return result;
}

Vector3 Matrix4x4::TransformVector3(const Vector3& vector, const Matrix4x4& matrix)
{
	Vector3 result{};
	result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0] +
		1.0f * matrix.m[3][0];
	result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1] +
		1.0f * matrix.m[3][1];
	result.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2] +
		1.0f * matrix.m[3][2];
	float w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] + vector.z * matrix.m[2][3] +
		1.0f * matrix.m[3][3];
	assert(w != 0.0f);
	result.x /= w;
	result.y /= w;
	result.z /= w;
	return result;
}

Matrix4x4 Matrix4x4::MakeTranspose(const Matrix4x4& m)
{
	Matrix4x4 result{};
	for (int y = 0; y < 4; y++) {
		for (int x = 0; x < 4; x++) {
			result.m[y][x] = m.m[x][y];
		}
	}
	return result;
}

Matrix4x4 Matrix4x4::MakeTranslateMatrix(const Vector3& translate)
{
	Matrix4x4 result = {};
	result.m[0][0] = 1;
	result.m[1][1] = 1;
	result.m[2][2] = 1;
	result.m[3][3] = 1;

	result.m[3][0] = translate.x;
	result.m[3][1] = translate.y;
	result.m[3][2] = translate.z;

	return result;
}

Matrix4x4 Matrix4x4::MakeScaleMatrix(const Vector3& scale)
{
	Matrix4x4 result = {};

	result.m[0][0] = scale.x;
	result.m[1][1] = scale.y;
	result.m[2][2] = scale.z;
	result.m[3][3] = 1;

	return result;
}

Matrix4x4 Matrix4x4::MakeRotateXMatrix(const float& radian)
{
	Matrix4x4 result = {};
	result.m[0][0] = 1.0f;
	result.m[1][1] = std::cosf(radian);
	result.m[1][2] = std::sinf(radian);
	result.m[2][1] = std::sinf(-radian);
	result.m[2][2] = std::cosf(radian);
	result.m[3][3] = 1.0f;
	return result;
}

Matrix4x4 Matrix4x4::MakeRotateYMatrix(const float& radian)
{
	Matrix4x4 result = {};
	result.m[0][0] = std::cosf(radian);
	result.m[0][2] = std::sinf(-radian);
	result.m[1][1] = 1.0f;
	result.m[2][0] = std::sinf(radian);
	result.m[2][2] = std::cosf(radian);
	result.m[3][3] = 1.0f;
	return result;
}

Matrix4x4 Matrix4x4::MakeRotateZMatrix(const float& radian)
{
	Matrix4x4 result = {};
	result.m[0][0] = std::cosf(radian);
	result.m[0][1] = std::sinf(radian);
	result.m[1][0] = std::sinf(-radian);
	result.m[1][1] = std::cosf(radian);
	result.m[2][2] = 1.0f;
	result.m[3][3] = 1.0f;
	return result;
}

Matrix4x4 Matrix4x4::MakeRotateXYZMatrix(const Vector3& rotate)
{
	// X
	Matrix4x4 matrixX = MakeRotateXMatrix(rotate.x);
	// Y
	Matrix4x4 matrixY = MakeRotateYMatrix(rotate.y);
	// Z
	Matrix4x4 matrixZ = MakeRotateZMatrix(rotate.z);
	// 回転行列
	Matrix4x4 matrixRotate = Multiply(matrixX, Multiply(matrixZ, matrixY));

	return matrixRotate;
}

Matrix4x4 Matrix4x4::MakeRotateMatrix(const Vector3& direction)
{
	// 方向ベクトルを正規化
	Vector3 normDirect = Vector3::Normalize(direction);

	// Z軸を基準としてY軸を選択（通常の上方向）
	Vector3 up(0.0f, 1.0f, 0.0f);

	// Z軸方向が真上か真下の場合に備えた調整
	if (std::fabs(normDirect.y) > 0.999f) {
		up = Vector3(1.0f, 0.0f, 0.0f); // 別の軸を使う
	}

	// 新しいX軸を生成（方向ベクトルとupベクトルの外積）
	Vector3 xAxis = Vector3::Normalize(Vector3::Cross(up, normDirect));

	// 新しいY軸を生成（Z軸（方向ベクトル）とX軸の外積）
	Vector3 yAxis = Vector3::Normalize(Vector3::Cross(normDirect, xAxis));

	// 4x4の回転行列を構築
	Matrix4x4 rotationMatrix{};
	rotationMatrix.m[3][3] = 1.0f;

	rotationMatrix.m[0][0] = xAxis.x; rotationMatrix.m[0][1] = yAxis.x; rotationMatrix.m[0][2] = normDirect.x;
	rotationMatrix.m[1][0] = xAxis.y; rotationMatrix.m[1][1] = yAxis.y; rotationMatrix.m[1][2] = normDirect.y;
	rotationMatrix.m[2][0] = xAxis.z; rotationMatrix.m[2][1] = yAxis.z; rotationMatrix.m[2][2] = normDirect.z;
	
	return rotationMatrix;
}

Matrix4x4 Matrix4x4::MakeRotateMatrix(const Quaternion& rotate)
{
	Matrix4x4 result = MakeIdentity4x4();
	Quaternion normalizeRotate = Quaternion::Normalize(rotate);
	result.m[0][0] = std::powf(normalizeRotate.w, 2) + std::powf(normalizeRotate.x, 2) - std::powf(normalizeRotate.y, 2) - std::powf(normalizeRotate.z, 2);
	result.m[0][1] = 2 * (normalizeRotate.x * normalizeRotate.y + normalizeRotate.w * normalizeRotate.z);
	result.m[0][2] = 2 * (normalizeRotate.x * normalizeRotate.z - normalizeRotate.w * normalizeRotate.y);
	result.m[1][0] = 2 * (normalizeRotate.x * normalizeRotate.y - normalizeRotate.w * normalizeRotate.z);
	result.m[1][1] = std::powf(normalizeRotate.w, 2) - std::powf(normalizeRotate.x, 2) + std::powf(normalizeRotate.y, 2) - std::powf(normalizeRotate.z, 2);
	result.m[1][2] = 2 * (normalizeRotate.y * normalizeRotate.z + normalizeRotate.w * normalizeRotate.x);
	result.m[2][0] = 2 * (normalizeRotate.x * normalizeRotate.z + normalizeRotate.w * normalizeRotate.y);
	result.m[2][1] = 2 * (normalizeRotate.y * normalizeRotate.z - normalizeRotate.w * normalizeRotate.x);
	result.m[2][2] = std::powf(normalizeRotate.w, 2) - std::powf(normalizeRotate.x, 2) - std::powf(normalizeRotate.y, 2) + std::powf(normalizeRotate.z, 2);
	return result;
}

Matrix4x4 Matrix4x4::MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate)
{
	// スケール
	Matrix4x4 scaleMat = MakeScaleMatrix(scale);
	// 回転
	Matrix4x4 rotateMat = MakeRotateXYZMatrix(rotate);
	// 平行移動
	Matrix4x4 translateMat = MakeTranslateMatrix(translate);

	Matrix4x4 affineMatrix = Multiply(scaleMat, Multiply(rotateMat, translateMat));

	return affineMatrix;
}

Matrix4x4 Matrix4x4::MakeAffineMatrix(const Vector3& scale, const Vector3& from, const Vector3& to, const Vector3& translate)
{
	// スケール
	Matrix4x4 scaleMat = MakeScaleMatrix(scale);
	// 回転
	Matrix4x4 rotateMat = DirectionToDirection(from, to);
	// 平行移動
	Matrix4x4 translateMat = MakeTranslateMatrix(translate);

	Matrix4x4 affineMatrix = Multiply(scaleMat, Multiply(rotateMat, translateMat));

	return affineMatrix;
}

Matrix4x4 Matrix4x4::MakeAffineMatrix(const Vector3& scale, const Quaternion& rotate, const Vector3& translate)
{
	Matrix4x4 affineMatrix = {};
	// スケール
	Matrix4x4 scaleMat = MakeScaleMatrix(scale);
	// 回転
	Matrix4x4 rotateMat = MakeRotateMatrix(rotate);
	// 平行移動
	Matrix4x4 translateMat = MakeTranslateMatrix(translate);

	affineMatrix = Multiply(scaleMat, Multiply(rotateMat, translateMat));

	return affineMatrix;
}

Matrix4x4 Matrix4x4::MakeUvTransformMatirx(const Vector3& scale, const Vector3& rotate, const Vector3& translate)
{
	Matrix4x4 uvMatrix = Matrix4x4::MakeScaleMatrix(scale);
	uvMatrix = Matrix4x4::Multiply(uvMatrix, Matrix4x4::MakeRotateZMatrix(rotate.z));
	uvMatrix = Matrix4x4::Multiply(uvMatrix, Matrix4x4::MakeTranslateMatrix(translate));
	return Matrix4x4(uvMatrix);
}

Matrix4x4 Matrix4x4::DirectionToDirection(const Vector3& from, const Vector3& to)
{
	// cos
	float cosine = Vector3::Dot(from, to);
	// sin
	float sine = Vector3::Length(Vector3::Cross(from, to));
	// normalize
	Vector3 axisNormal = {};
	Vector3 fromMinus = Vector3::Scaler(from, -1.0f);
	if (fromMinus.x == to.x && fromMinus.y == to.y && fromMinus.z == to.z) {
		if (from.x != 0 || from.y != 0) {
			axisNormal = { from.y,-from.x,0 };
		}
		else if (from.x != 0 || from.z != 0) {
			axisNormal = { from.z,0,-from.x };
		}
	}
	else {
		axisNormal = Vector3::Normalize(Vector3::Cross(from, to));
	}
	Matrix4x4 result = MakeIdentity4x4();
	result.m[0][0] = std::powf(axisNormal.x, 2) * (1 - cosine) + cosine;
	result.m[0][1] = axisNormal.x * axisNormal.y * (1 - cosine) + axisNormal.z * sine;
	result.m[0][2] = axisNormal.x * axisNormal.z * (1 - cosine) - axisNormal.y * sine;

	result.m[1][0] = axisNormal.x * axisNormal.y * (1 - cosine) - axisNormal.z * sine;
	result.m[1][1] = std::powf(axisNormal.y, 2) * (1 - cosine) + cosine;
	result.m[1][2] = axisNormal.y * axisNormal.z * (1 - cosine) + axisNormal.x * sine;

	result.m[2][0] = axisNormal.x * axisNormal.z * (1 - cosine) + axisNormal.y * sine;
	result.m[2][1] = axisNormal.y * axisNormal.z * (1 - cosine) - axisNormal.x * sine;
	result.m[2][2] = std::powf(axisNormal.z, 2) * (1 - cosine) + cosine;

	return result;
}

Matrix4x4 Matrix4x4::MakeOrthographicMatrix(const float& left, const float& top, const float& right, const float& bottom, const float& nearClip, const float& farClip)
{
	Matrix4x4 result = {};
	result.m[0][0] = 2 / (right - left);
	result.m[1][1] = 2 / (top - bottom);
	result.m[2][2] = 1 / (farClip - nearClip);
	result.m[3][0] = (left + right) / (left - right);
	result.m[3][1] = (top + bottom) / (bottom - top);
	result.m[3][2] = nearClip / (nearClip - farClip);
	result.m[3][3] = 1;
	return result;
}

Matrix4x4 Matrix4x4::MakeViewportMatrix(const float& left, const float& top, const float& width, const float& height, const float& minDepth, const float& maxDepth)
{
		Matrix4x4 result = {};
		result.m[0][0] = width / 2;
		result.m[1][1] = -height / 2;
		result.m[2][2] = maxDepth - minDepth;
		result.m[3][0] = left + (width / 2);
		result.m[3][1] = top + (height / 2);
		result.m[3][2] = minDepth;
		result.m[3][3] = 1;
		return Matrix4x4(result);
}

Matrix4x4 Matrix4x4::MakePerspectiveFovMatrix(const float& fovY, const float& aspectRatio, const float& nearClip, const float& farClip)
{
	Matrix4x4 result = {};

	result.m[0][0] = (1.0f / aspectRatio) * (1.0f / tanf(fovY / 2));
	result.m[1][1] = (1.0f / tanf(fovY / 2));
	result.m[2][2] = (farClip) / (farClip - nearClip);
	result.m[2][3] = 1;
	result.m[3][2] = (-nearClip * farClip) / (farClip - nearClip);

	return result;
}

Matrix4x4 Matrix4x4::MakeBillBoardMatrix(const Matrix4x4& cameraMatrix)
{
	Matrix4x4 backToFrontMatrix = MakeRotateYMatrix(std::numbers::pi_v<float>);
	Matrix4x4 billBoardMatrix = Multiply(backToFrontMatrix, cameraMatrix);
	billBoardMatrix.m[3][0] = 0.0f;
	billBoardMatrix.m[3][1] = 0.0f;
	billBoardMatrix.m[3][2] = 0.0f;
	return Matrix4x4(billBoardMatrix);
}
Matrix4x4 Matrix4x4::MakeBillBoardMatrix(const Vector3& target, const Vector3& eye, const Vector3& up)
{
	// 回転行列
	// X軸
	Vector3 zAxis = Vector3::Normalize(target-eye);
	// Y軸
	Vector3 xAxis = Vector3::Normalize(Vector3::Cross(up, zAxis));
	// Z軸
	Vector3 yAxis = Vector3::Cross(zAxis, xAxis);

	Matrix4x4 result{};

	result.m[0][0] = xAxis.x;
	result.m[0][1] = xAxis.y;
	result.m[0][2] = xAxis.z;
	result.m[1][0] = yAxis.x;
	result.m[1][1] = yAxis.y;
	result.m[1][2] = yAxis.z;
	result.m[2][0] = zAxis.x;
	result.m[2][1] = zAxis.y;
	result.m[2][2] = zAxis.z;
	result.m[3][3] = 1.0f;

	return result;
}

Matrix4x4 Matrix4x4::MakeBillBoardMatrix(const Vector3& target, const Vector3& eye, const Vector3& scale, const Vector3& up)
{
	// 回転行列
		// Z軸
	Vector3 zAxis = Vector3::Normalize(target - eye);

	// 修正されたY軸（アップベクトルがZ軸と平行でないようにする）
	Vector3 correctedUp = Vector3::Normalize(up);
	if (Vector3::Dot(correctedUp, zAxis) > 0.99f) {
		correctedUp = { 0.0f, 1.0f, 0.0f }; // デフォルトの上方向
	}

	// X軸
	Vector3 xAxis = Vector3::Normalize(Vector3::Cross(correctedUp, zAxis));
	// Y軸
	Vector3 yAxis = Vector3::Cross(zAxis, xAxis);

	// 行列の設定
	Matrix4x4 result{};

	// 各軸にスケールを適用
	result.m[0][0] = xAxis.x * scale.x;
	result.m[0][1] = xAxis.y * scale.x;
	result.m[0][2] = xAxis.z * scale.x;
	result.m[1][0] = yAxis.x * scale.y;
	result.m[1][1] = yAxis.y * scale.y;
	result.m[1][2] = yAxis.z * scale.y;
	result.m[2][0] = zAxis.x * scale.z;
	result.m[2][1] = zAxis.y * scale.z;
	result.m[2][2] = zAxis.z * scale.z;

	// 平行移動を追加
	result.m[3][0] = target.x;
	result.m[3][1] = target.y;
	result.m[3][2] = target.z;
	result.m[3][3] = 1.0f;

	return result;
}
