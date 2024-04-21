#pragma once
#include "../Math/MathLib.h"

struct CBufferTransformMatrix
{
	Matrix4x4 WVP;
	Matrix4x4 world;
};

struct VertexData
{
	Vector4 position;
	Vector2 texcoord;
	Vector3 normal;
};

struct ModelData {
	VertexData vertices;
};

//struct MaterialData {
//	
//};
