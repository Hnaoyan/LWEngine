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

}
