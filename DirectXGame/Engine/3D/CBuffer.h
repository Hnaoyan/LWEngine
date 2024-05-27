#pragma once
#include "../Math/MathLib.h"

/// <summary>
/// 全体的に使うトランスフォーム構造体
/// </summary>
struct Transform {
	Vector3 scale;
	Vector3 rotate;
	Vector3 translate;
};

struct CBufferWorldTransform {
	Matrix4x4 world;
};

#pragma region Model

struct VertexData
{
	Vector4 position;	// 座標
	Vector2 texcoord;	// テクすコード
	Vector3 normal;		// 法線ベクトル
};

#pragma endregion


//struct ModelData {
//	VertexData vertices;	// 頂点データ
//};

// カメラ用
struct CBufferDataCamera {
	Matrix4x4 viewMatrix;		// ビュー行列
	Matrix4x4 projectionMatrix;	// プロジェクション行列
	Vector3 worldPosition;		// ワールド座標
};

// マテリアル用
struct CBufferDataMaterial {
	Vector4 color;			// カラー
	int32_t enableLighting;	// ライティングのswitch
};
