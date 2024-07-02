#pragma once
#include "../Math/MathLib.h"

struct CBufferDataBlur {
	Vector2 centerPoint;	// 中心座標
	int32_t samplesNum;	// サンプリング数
	float blurWidth;	// ぼかしの幅
};

struct CBufferDataVignette {
	Vector3 color;	// ビネットの色
	float scale;	// 幅のスケール
	float powValue;	// 累乗の値
};

struct CBufferDataDissolve
{
	Vector3 color;	// エッジの色
	float threshold;// 閾値
};
