#pragma once
#include <memory>
#include <cassert>
#include <type_traits>
#include <concepts>

#include "Engine/Math/MathLib.h"

struct EmitterSphere {
	Vector3 translate;	// 位置
	// 射出関係
	float radius;		// 半径
	uint32_t count;		// 生成数
	float frequency;	// 間隔
	float frequencyTime; // 間隔時間
	uint32_t emit;	// 許可フラグ
	uint32_t emitPattern;	// 生成のパターン
};

struct PerFrame {
	// 起動してからの時間
	float time;
	float deltaTime;
};

struct ParticleCS {
	Vector3 translate;	// 位置
	Vector3 scale;	// サイズ
	Vector3 rotate;	// 回転
	float lifetime;		// 生存時間
	Vector3 velocity;	// 速度
	float currentTime;	// 経過時間
	float startAlpha;
	Vector4 color;	// 色
	uint32_t isScaleDecrement;	// スケール変更フラグ
	uint32_t isGravity;	// 重力フラグ
	uint32_t alphaDecrement;	// アルファ値の減少タイプ
};

struct ParticleGPU {
	Matrix4x4 worldMatrix;
	Vector4 color;
};

struct PerView {
	Matrix4x4 viewMatrix;
	Matrix4x4 projectionMatrix;
	Matrix4x4 billBoardMatrix;
};

