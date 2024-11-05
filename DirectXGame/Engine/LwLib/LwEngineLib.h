#pragma once

// デルタタイム
#include "DeltaTime.h"
#include "../Math/MathLib.h"
#include "../Camera/CameraList.h"

#include <algorithm>
#include <random>
#include <numbers>

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
	inline static float GetRandomValue(const float& min, const float& max) {
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

	inline static float GetRandomValue(const float& min, const float& max, const float& limit) {
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
	inline static float Lerp(const float& a, const float& b, const float& t, float deadZone = 0.00001f) {
		float clampT = std::clamp(t, 0.0f, 1.0f);
		float result = (1.0f - clampT) * a + clampT * b;
		//float result = a + clampT * (b - a);
		// 例外処理
		if (std::fabsf(result) <= deadZone) {
			return 0.0f;
		}
		return result;
	}

	template<typename T>
	inline static T LerpToCurrent(const T& currentValue, const T& targetValue, const float& t) {
		return currentValue + (targetValue - currentValue) * t;
	}

	//inline static Vector3 Slerp(const Vector3& start, const Vector3& end, float t) {
	//	float dot = Vector3::Dot(start, end);
	//	float theta = (float)acos((dot * (float)std::numbers::pi) / 180);
	//	float sinTheta = (float)sin(theta);
	//	float weightStart = (float)sin((1 - t) * theta) / sinTheta;
	//	float weightEnd = (float)sin(t * theta) / sinTheta;

	//	Vector3 result;
	//	result.x = (weightStart * start.x + weightEnd * end.x);
	//	result.y = (weightStart * start.y + weightEnd * end.y);
	//	result.z = (weightStart * start.z + weightEnd * end.z);
	//	return result;
	//}

	/// <summary>
	/// 球面線形補間
	/// </summary>
	/// <param name="moveVector"></param>
	/// <param name="targetVector"></param>
	/// <param name="t"></param>
	/// <returns></returns>
	static Vector3 Slerp(const Vector3& moveVector, const Vector3& targetVector, const float& t)
	{
		Vector3 result = {};
		Vector3 mvNormalize = Vector3::Normalize(moveVector);
		Vector3 trNormalize = Vector3::Normalize(targetVector);

		float dot = Vector3::Dot(mvNormalize, trNormalize);

		// 逆向きに非常に近い場合
		if (dot < -0.999f) {
			// ここでは、特定の軸に沿って回転する処理に変更できる
			result = mvNormalize* (1.0f - t);
			result += trNormalize * t;
			return Vector3::Normalize(result);
		}

		// 同方向に非常に近い場合
		if (dot > 0.999f) {
			return moveVector; // すでに同じ方向なのでそのまま返す
		}
		// 角度
		float theta = std::acosf(dot);
		float sinTheta = std::sinf(theta);
		// 分母が非常に小さい場合の安全処理
		if (sinTheta < 0.0001f) {
			return moveVector; // 回転不要
		}
		float s0 = std::sinf((1.0f - t) * theta) / sinTheta;
		float s1 = std::sinf(t * theta) / sinTheta;

		result = (mvNormalize * s0) + (trNormalize * s1);

		return Vector3::Normalize(result);
	}
	/// <summary>
	/// 角度補間
	/// </summary>
	/// <param name="a"></param>
	/// <param name="b"></param>
	/// <param name="t"></param>
	/// <returns></returns>
	inline static float LerpShortAngle(const float& a, const float& b, const float& t) {
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
	inline static float RatioCalc(const float& min, const float& max, const float& rate) {
		return min + (max - min) * rate;
	}

	/// <summary>
	/// 角度からラジアンに
	/// </summary>
	/// <param name="angle"></param>
	/// <returns></returns>
	inline static float AngleToRadian(const float& angle) {
		return float(angle * ((float)std::numbers::pi / 180.0f));
	}

	/// <summary>
	/// ラジアンから角度に
	/// </summary>
	/// <param name="radian"></param>
	/// <returns></returns>
	inline static float RadianToAngle(const float& radian) {
		return float(radian * (180.0f / (float)std::numbers::pi));
	}

	inline static float CalculateYawFromVector(const Vector3& direction)
	{
		// 任意軸との内積
		float dot = Vector3::Dot(direction, Vector3(0, 0, 1.0f));
		// 方向ベクトルの長さ
		float fromLength = Vector3::Length(direction);
		// 任意軸ベクトルの長さ
		float toLength = Vector3::Length(Vector3(0, 0, 1.0f));
		// 上記を使ってacosで
		float result = std::acosf(dot / (fromLength * toLength));

		if (direction.x < 0) {
			result = -result;
		}

		return result;
	}

	// 指数関数補間
	// currrent:現在
	// end:終了
	// decay:減衰値
	inline static float ExponentialInterpolate(const float& current, const float& end, float decay, float deltaTime) {
		float factor = 1.0f - std::exp(-decay * deltaTime);
		return current + (end - current) * factor;
	}
	inline static Vector2 ExponentialInterpolate(const Vector2& current, const Vector2& end, const float& decay, const float& deltaTime) {
		float factor = 1.0f - std::exp(-decay * deltaTime);
		return current + (end - current) * factor;
	}
	inline static Vector3 ExponentialInterpolate(const Vector3& current, const Vector3& end, const float& decay, const float& deltaTime) {
		float factor = 1.0f - std::exp(-decay * deltaTime);
		return current + (end - current) * factor;
	}

#pragma region 2軸回転
	// XZ平面回転
	inline static Vector3 RotateXZVector(const Vector3& direct, const float& theta)
	{
		Vector3 normalizeVector = Vector3::Normalize(direct);
		Matrix3x3 rotateMatrix = Matrix3x3::MakeRotateMatrix(theta);
		Vector2 newDirect = Matrix3x3::Transform({ normalizeVector.x,normalizeVector.z }, rotateMatrix);
		return Vector3(newDirect.x, normalizeVector.y, newDirect.y);
	}
	// XY平面回転
	inline static Vector3 RotateXYVector(const Vector3& direct, const float& theta) {
		Vector3 normalizeVector = Vector3::Normalize(direct);
		Matrix3x3 rotateMatrix = Matrix3x3::MakeRotateMatrix(theta);
		Vector2 newDirect = Matrix3x3::Transform({ normalizeVector.x,normalizeVector.y }, rotateMatrix);
		return Vector3(newDirect.x, newDirect.y, normalizeVector.z);
	}
	// YZ平面回転
	inline static Vector3 RotateYZVector(const Vector3& direct, const float& theta) {
		Vector3 normalizeVector = Vector3::Normalize(direct);
		Matrix3x3 rotateMatrix = Matrix3x3::MakeRotateMatrix(theta);
		Vector2 newDirect = Matrix3x3::Transform({ normalizeVector.y,normalizeVector.z }, rotateMatrix);
		return Vector3(normalizeVector.x, newDirect.x, newDirect.y);
	}
#pragma endregion

	static Vector3 HomingAccelerate(const Vector3& position, const Vector3& target, const Vector3& velocity, const float& period) {
		Vector3 result = {};
		Vector3 diff = target - position;
		result += (diff - velocity * period) * (2.0f / (period * period));
		
		return result;
	}

}
