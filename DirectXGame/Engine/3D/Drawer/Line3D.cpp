#include "Line3D.h"
#include "Engine/Base/DirectXCommon.h"

void Line3D::Line::Initialize()
{
	ID3D12Device* device = DirectXCommon::GetInstance()->GetDevice();

	vertex_.cBuffer_.CreateConstantBuffer(device);

}
