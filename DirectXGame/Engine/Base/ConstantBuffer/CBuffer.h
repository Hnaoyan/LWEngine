#pragma once
#include "Engine/Math/MathLib.h"

// オイラートランスフォーム
struct EulerTransform {
	Vector3 scale;
	Vector3 rotate;
	Vector3 translate;
};

// クォータニオントランスフォーム
struct QuaternionTransform {
	Vector3 scale;
	Quaternion rotate;
	Vector3 translate;
};

// ワールドトランスフォーム
struct CBufferWorldTransform {
	Matrix4x4 world;
	Matrix4x4 worldInverseTranspose;
};

// トレイル頂点
struct TrailVertex
{
	Vector4 position;
	Vector4 color;
};

// 三角形データ
struct TriangleData 
{
	Vector3 position;	// 座標
	Vector4 color;		// 色
	Vector2 texcoord;	// テクスコード
};

#pragma region Model
// 頂点データ
struct VertexData
{
	Vector4 position;	// 座標
	Vector3 normal;		// 法線ベクトル
	Vector2 texcoord;	// テクすコード
};

// 線データ
struct LineData {
	Vector3 position;
	Vector4 color;
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
	float padding;
	// Dissolve用
	Vector3 dissolveColor;	// エッジの色
	float dissolveThreshold;	// Dissolveの閾値
	float discardThreshold;	// ディスカードの閾値
};

// 軌跡用のマテリアル
struct CBufferDataTrailMaterial {
	Vector4 color;			// 色
	Matrix4x4 uvTransform;	// UV
	float discardThreshold_;	// ディスカード
};

// インスタンシング用
struct CBufferInstancedUnit
{
	Matrix4x4 worldMatrix;
	Matrix4x4 worldInverseTranspose;
	Vector4 color;
	uint32_t instancedNum;
};
