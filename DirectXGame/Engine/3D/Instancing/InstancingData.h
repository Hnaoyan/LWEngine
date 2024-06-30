#pragma once
#include "../../Math/MathLib.h"

struct CBufferInstancedUnit
{
	Matrix4x4 worldMatrix;
	Matrix4x4 worldInverseTranspose;
	uint32_t instancedNum;
};
