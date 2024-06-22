#pragma once
#include "../Math/MathLib.h"

#pragma region Sprite
struct SpriteVertexData
{
	Vector3 position;	// 座標
	Vector2 texcoord;	// 
	//Vector3 normal;
};

struct SpriteDataForGPU {
	Vector4 color;	// 色
	Matrix4x4 world;// ワールド行列
	Matrix4x4 WVP;	// ワールドビュープロジェクション行列
	Matrix4x4 uvTransform;	// UV
};
#pragma endregion
