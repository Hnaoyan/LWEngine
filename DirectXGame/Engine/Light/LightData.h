#pragma once
#include "../Math/MathLib.h"

enum EnableLighting
{
	kNone,			// なし
	kDirectional,	// 方向ライト
	kSpot,			// スポットライト
	kMaxSize,		// サイズ
};

struct CBufferDataDirectionalLight {
	Vector4 color;
	Vector3 direction;
	float intencity;
};

//struct CBufferDataSpotLight
//{
//
//};