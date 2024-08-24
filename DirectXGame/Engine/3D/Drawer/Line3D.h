#pragma once
#include "Engine/Base/CBufferCommon.h"

namespace Line3D
{
	class LVertex {
	public:
		ConstantBufferMapContext<VertexLineData> cBuffer_;


	};

	class Line {
	public:
		//static Line* Create();
		void Initialize();
		void Update();

	private:
		LVertex vertex_;

	};

}
