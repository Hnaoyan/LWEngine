#pragma once
#include "../Math/MathLib.h"

struct CBufferDataBlur {
	Vector2 centerPoint;	// 中心座標
	float blurWidth;	// ぼかしの幅
	int32_t samplesNum;	// サンプリング数
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

struct CBufferDataNoise
{
	int32_t enableScreen;	// 画面にかけるかノイズのみか
	float time;	// 変化時間
};

struct CBufferDataHSV
{
	float hue;			// H
	float saturation;	// S
	float value;		// V
};

struct CBufferDataBloom
{
	float threshold;
	float sigma;
};
