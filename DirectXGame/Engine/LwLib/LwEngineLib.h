#pragma once

// デルタタイム
#include "DeltaTime.h"
#include "../Math/MathLib.h"
#include "../Camera/CameraList.h"

#include <algorithm>
#include <random>

namespace LwLib
{
	// ランダムエンジン
	static std::mt19937 sRandomEngine;
	/// <summary>
	/// 乱数用の初期化
	/// </summary>
	static void CreateRandomEngine() {
		std::random_device seedGenerator;
		sRandomEngine = std::mt19937(seedGenerator());
	}
	/// <summary>
	/// float型の乱数
	/// </summary>
	/// <param name="min"></param>
	/// <param name="max"></param>
	/// <returns></returns>
	inline static float GetRandomValue(float min, float max) {
		std::uniform_real_distribution<float> distribution(min, max);
		return distribution(sRandomEngine);
	}

	/// <summary>
	/// ワールドからスクリーン
	/// </summary>
	/// <param name="world"></param>
	/// <param name="camera"></param>
	/// <returns></returns>
	static Vector2 WorldToScreen(const Vector3& world, const ICamera* camera)
	{
		// ビューポート行列
		Matrix4x4 matViewport = Matrix4x4::MakeViewportMatrix(0, 0,
			(float)WindowAPI::kWindowWidth, (float)WindowAPI::kWindowHeight, 0, 1);
		// ビュープロジェクション
		Matrix4x4 matViewProjection = Matrix4x4::Multiply(
			Matrix4x4::Multiply(camera->viewMatrix_, camera->projectionMatrix_), matViewport);
		// スクリーン座標に変換
		Vector3 result = Matrix4x4::TransformVector3(world, matViewProjection);
		// ２次元ベクトルとして返す
		return Vector2(result.x, result.y);
	}

	/// <summary>
	/// ラープ
	/// </summary>
	/// <param name="a"></param>
	/// <param name="b"></param>
	/// <param name="t"></param>
	/// <returns></returns>
	inline static float Lerp(float a, float b, float t, float deadZone = 0.00001f) {
		float clampT = std::clamp(t, 0.0f, 1.0f);
		float result = (1.0f - clampT) * a + clampT * b;
		//float result = a + clampT * (b - a);
		// 例外処理
		if (std::fabsf(result) <= deadZone) {
			return 0.0f;
		}
		return result;
	}

	/// <summary>
	/// 角度補間
	/// </summary>
	/// <param name="a"></param>
	/// <param name="b"></param>
	/// <param name="t"></param>
	/// <returns></returns>
	inline static float LerpShortAngle(float a, float b, float t) {
		// 角度差分
		float diff = b - a;

		// 角度を[-PI, +PI]に補正
		if (diff > 2.0f * float(std::numbers::pi) || diff < -2.0f * float(std::numbers::pi)) {
			diff = std::fmodf(diff, 2.0f * float(std::numbers::pi));
		}
		if (diff > float(std::numbers::pi)) {
			diff += -2.0f * float(std::numbers::pi);
		}
		else if (diff < -float(std::numbers::pi)) {
			diff += -2.0f * float(std::numbers::pi);
		}
		//diff = std::fmodf(diff + (float(std::numbers::pi)), 2.0f * (float(std::numbers::pi))) - (float(std::numbers::pi));
		// Lerp
		diff = Lerp(a, diff, t);
		return diff;
	}

	/// <summary>
	/// 割合計算
	/// </summary>
	/// <param name="min"></param>
	/// <param name="max"></param>
	/// <param name="rate"></param>
	/// <returns></returns>
	inline static float RatioCalc(float min, float max, float rate) {
		return min + (max - min) * rate;
	}

	inline static float CalculateYawFromVector(const Vector3& direction)
	{
		float dot = Vector3::Dot(direction, Vector3(0, 0, 1.0f));
		float fromLength = Vector3::Length(direction);
		float toLength = Vector3::Length(Vector3(0, 0, 1.0f));
		float result = std::acosf(dot / (fromLength * toLength));

		if (direction.x < 0) {
			result = -result;
		}

		return result;
	}
}
