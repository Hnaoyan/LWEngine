#pragma once
#include "Engine/Base/CBufferCommon.h"
#include "Engine/3D/ModelUtility/ModelData.h"

#include <vector>
#include <optional>

class Trail3D
{
public:
	void Initialize();
	void Update(std::vector<Vector3> controlPoint);

private:
	void TransferVertex();
	void CreateVertex();
	
};
