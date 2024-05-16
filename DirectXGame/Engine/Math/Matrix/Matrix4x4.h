#pragma once

class Matrix4x4
{
public:
	float m[4][4];

public:
	static Matrix4x4 MakeIdentity4x4();
	static Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);

public:
	static Matrix4x4 MakeOrthographicMatrix(
		float left, float top, float right, float bottom, float nearClip, float farClip);

};
