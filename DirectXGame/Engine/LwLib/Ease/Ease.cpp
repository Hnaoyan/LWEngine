#include "Ease.h"
#include <cmath>

float Ease::EaseT(EaseType type, const float& t)
{
	switch (type)
	{
	case Ease::EaseType::kDefault:
		return t;
		break;
	case Ease::EaseType::kEaseInQuadF:
		return t * t;
		break;
	case Ease::EaseType::kEaseOutQuadF:
		return 1.0f - std::powf(1.0f - t, 2.0f);
		break;
	case Ease::EaseType::kEaseInOutQuadF:
		return t > 0.5f ? 2.0f * t * t : 1.0f - std::powf(-2.0f * t + 2.0f, 2.0f) / 2.0f;
		break;
	case Ease::EaseType::kEaseInCubicF:
		return t * t * t;
		break;
	case Ease::EaseType::kEaseOutCubicF:
		return 1.0f - std::powf(1.0f - t, 3.0f);
		break;
	case Ease::EaseType::kEaseInOutCubicF:
		return t > 0.5f ? 4.0f * t * t * t : 1.0f - std::powf(-2.0f * t + 2.0f, 3.0f) / 2.0f;
		break;
	}
    return 0.0f;
}
