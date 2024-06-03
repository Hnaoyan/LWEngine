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
};
