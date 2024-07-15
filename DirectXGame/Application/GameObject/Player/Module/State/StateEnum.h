#pragma once
#include <stdint.h>

enum class VerticalStates : int32_t {
	kIdle,
	kJump,
	kFall,
};

enum class HorizontalStates : int32_t {
	kIdle,
	kMove,
	kBoost,
};
