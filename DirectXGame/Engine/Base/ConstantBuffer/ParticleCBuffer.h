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
	uint32_t count;		// 数
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
	Vector3 translate;
	Vector3 scale;
	float lifetime;
	Vector3 velocity;
	float currentTime;
	Vector4 color;
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

