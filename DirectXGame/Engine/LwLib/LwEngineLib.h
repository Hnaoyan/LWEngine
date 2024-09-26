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

#pragma region 乱数
	inline static int GetRandomValue(int min, int max) {
		CreateRandomEngine();
		std::uniform_int_distribution<int> distribution(min, max);
		return distribution(sRandomEngine);
	}
	inline static float GetRandomValue(float min, float max) {
		CreateRandomEngine();
		std::uniform_real_distribution<float> distribution(min, max);
		return distribution(sRandomEngine);
	}
	inline static Vector2 GetRandomValue(const Vector2& min, const Vector2& max) {
		return Vector2(GetRandomValue(min.x, max.x), GetRandomValue(min.y, max.y));
	}
	inline static Vector3 GetRandomValue(const Vector3& min, const Vector3& max) {
		return Vector3(GetRandomValue(min.x, max.x), GetRandomValue(min.y, max.y), GetRandomValue(min.z, max.z));
	}

	inline static float GetRandomValue(float min, float max, float limit) {
		CreateRandomEngine();
		std::uniform_real_distribution<float> distribution(min, max);
		float value = distribution(sRandomEngine);
		if (value >= 0) {
			value = std::clamp(value, limit, max);
		}
		else {
			value = std::clamp(value, -max, -limit);
		}
		return value;
	}
	inline static Vector2 GetRandomValue(const Vector2& min, const Vector2& max, float limit) {
		return Vector2(GetRandomValue(min.x, max.x, limit), GetRandomValue(min.y, max.y, limit));
	}
	inline static Vector3 GetRandomValue(const Vector3& min, const Vector3& max, float limit) {
		return Vector3(GetRandomValue(min.x, max.x, limit), GetRandomValue(min.y, max.y, limit), GetRandomValue(min.z, max.z, limit));
	}
#pragma endregion

	class Curve {
	public:
		/// <summary>
		/// ベジェ曲線
		/// </summary>
		/// <param name="p0"></param>
		/// <param name="p1"></param>
		/// <param name="p2"></param>
		/// <param name="t"></param>
		/// <returns></returns>
		inline static Vector2 BezierCurve(const Vector2& p0, const Vector2& p1, const Vector2& p2, float t) {
			Vector2 p0p1 = Vector2::Lerp(p0, p1, t);
			Vector2 p1p2 = Vector2::Lerp(p1, p2, t);
			return Vector2(Vector2::Lerp(p0p1, p1p2, t));
		}


	};

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

#pragma region 2軸回転
	// XZ平面回転
	inline static Vector3 RotateXZVector(const Vector3& direct, float theta)
	{
		Vector3 normalizeVector = Vector3::Normalize(direct);
		Matrix3x3 rotateMatrix = Matrix3x3::MakeRotateMatrix(theta);
		Vector2 newDirect = Matrix3x3::Transform({ normalizeVector.x,normalizeVector.z }, rotateMatrix);
		return Vector3(newDirect.x, normalizeVector.y, newDirect.y);
	}
	// XY平面回転
	inline static Vector3 RotateXYVector(const Vector3& direct, float theta) {
		Vector3 normalizeVector = Vector3::Normalize(direct);
		Matrix3x3 rotateMatrix = Matrix3x3::MakeRotateMatrix(theta);
		Vector2 newDirect = Matrix3x3::Transform({ normalizeVector.x,normalizeVector.y }, rotateMatrix);
		return Vector3(newDirect.x, newDirect.y, normalizeVector.z);
	}
	// YZ平面回転
	inline static Vector3 RotateYZVector(const Vector3& direct, float theta) {
		Vector3 normalizeVector = Vector3::Normalize(direct);
		Matrix3x3 rotateMatrix = Matrix3x3::MakeRotateMatrix(theta);
		Vector2 newDirect = Matrix3x3::Transform({ normalizeVector.y,normalizeVector.z }, rotateMatrix);
		return Vector3(normalizeVector.x, newDirect.x, newDirect.y);
	}
#pragma endregion
}
