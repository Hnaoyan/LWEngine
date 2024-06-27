#pragma once

// デルタタイム
#include "DeltaTime.h"
#include "../Math/MathLib.h"
#include "../Camera/CameraList.h"

#include <algorithm>

namespace LwLib
{
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
	inline static float Lerp(float a, float b, float t) {
		float clampT = std::clamp(t, 0.0f, 1.0f);
		return (1.0f - clampT) * a + clampT * b;
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
		diff = std::fmodf(diff, 2.0f * float(std::numbers::pi));
		// 角度を[-PI, +PI]に補正
		diff = std::fmodf(diff + (float(std::numbers::pi)), 2.0f * (float(std::numbers::pi))) - (float(std::numbers::pi));
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

}
