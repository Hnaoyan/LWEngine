#pragma once
#include "Engine/Math/MathLib.h"

// ライティングの種類
enum EnableLighting
{
	kNone,			// なし
	kAllLighting,	// 全て
	kDirectional,	// 平行光源
	kPoint,			// 点光源
	kSpot,			// スポットライト
	kMaxSize,		// サイズ
};

// 平行光源
struct CBufferDataDirectionalLight {
	Vector4 color;	// ライトの色
	Vector3 direction;	// ライトの方向
	float intensity;	// 輝度
};

// 点光源
struct CBufferDataPointLight {
	Vector4 color;	// ライトの色
	Vector3 position;	// 位置
	float intensity;	// 輝度
	float radius;	// 届く距離
	float decay;	// 減衰率
	float padding[2];	// 合わせ
};

// 方向光源
struct CBufferDataSpotLight
{
	Vector4 color;	// ライトの色
	Vector3 position;	// ライトの位置
	float intensity;	// 輝度
	Vector3 direction;	// スポットライトの方向
	float distance;	// ライトの届く距離
	float decay;	// 減衰率
	float cosAngle;	// スポットライトの余弦
	float cosFalloffStart; // 開始角度
	float padding[2];
};