#pragma once
#include <stdint.h>

class Ease
{
public:
	enum class EaseType : uint32_t
	{
		kDefault,
		kEaseInQuadF,
		kEaseOutQuadF,
		kEaseInOutQuadF,
		kEaseInCubicF,
		kEaseOutCubicF,
		kEaseInOutCubicF,
	};

	/// <summary>
	/// イージング処理
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="start"></param>
	/// <param name="end"></param>
	/// <param name="t"></param>
	/// <param name="type"></param>
	/// <returns></returns>
	template<typename T>
	static T Easing(const T& start, const T& end, const float& t, EaseType type = EaseType::kDefault);

	/// <summary>
	/// イージングの補間の値取得関数
	/// </summary>
	/// <param name="type"></param>
	/// <param name="t"></param>
	/// <returns></returns>
	static float EaseT(EaseType type, const float& t);

};

template<typename T>
inline T Ease::Easing(const T& start, const T& end, const float& t, EaseType type)
{
	float easeT = EaseT(type, t);
	return T((start * (1.0f - easeT)) + (end * easeT));
}
