#pragma once
#include "../MathLib.h"

class VectorLib
{
public:
	template <typename vType>
	inline static vType Lerp(vType start, vType end,float t) {
		vType result;
		result = start + (end - start) * t;
		return result;
	}

	//inline static Vector3 Lerp(const Vector3& start, const Vector3& end, float t) {
	//	return {
	//		(1.0f - t) * start.x + t * end.x,
	//		(1.0f - t) * start.y + t * end.y,
	//		(1.0f - t) * start.z + t * end.z,
	//	};
	//}

};
