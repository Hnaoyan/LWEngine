#pragma once
#include "Engine/Base/CBufferCommon.h"

namespace Line3D
{
	class LVertex {
	public:
		ConstantBufferMapContext<VertexLineData> vertex_;


	};

	class LMaterial {
	public:



	};

	class Line {
	public:
		static Line* Create();
		void Initialize();
		void Update();

	};

}
