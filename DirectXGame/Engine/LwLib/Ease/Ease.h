#pragma once
#include <stdint.h>

class Ease
{
public:
	enum class EaseType : uint32_t
	{
		kDefault,
		kEaseInQuadF,
		kEaseOutQuedF,
	};

	template<typename T>
	static T Easing(const T& start, const T& end, float t, EaseType type = EaseType::kDefault);

	static float EaseT(EaseType type, float t);

};

template<typename T>
inline T Ease::Easing(const T& start, const T& end, float t, EaseType type)
{
	float easeT = EaseT(type, t);
	return T();
}
