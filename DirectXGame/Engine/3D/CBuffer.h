#pragma once
#include "../Math/MathLib.h"

/// <summary>
/// 全体的に使うオイラートランスフォーム構造体
/// </summary>
struct EulerTransform {
	Vector3 scale;
	Vector3 rotate;
	Vector3 translate;
};

/// <summary>
/// クォータニオントランスフォーム
/// </summary>
struct QuaternionTransform {
	Vector3 scale;
	Quaternion rotate;
	Vector3 translate;
};


struct CBufferWorldTransform {
	Matrix4x4 world;
	Matrix4x4 worldInverseTranspose;
};

#pragma region Model

struct VertexData
{
	Vector4 position;	// 座標
	Vector3 normal;		// 法線ベクトル
	Vector2 texcoord;	// テクすコード
};

#pragma endregion

// カメラ用
struct CBufferDataCamera {
	Matrix4x4 viewMatrix;		// ビュー行列
	Matrix4x4 projectionMatrix;	// プロジェクション行列
	Vector3 worldPosition;		// ワールド座標
};

// マテリアル用
struct CBufferDataMaterial {
	Vector4 color;			// カラー
	Matrix4x4 uvTransform;	// UV
	int32_t enableLighting;	// ライティングのswitch
	float shininess;	// 輝度
	float coefficient;	// 環境マップの係数
	float threshold;	// Dissolveの閾値

};

// インスタンシング用
struct CBufferInstancedUnit
{
	Matrix4x4 worldMatrix;
	Matrix4x4 worldInverseTranspose;
	uint32_t instancedNum;
};
