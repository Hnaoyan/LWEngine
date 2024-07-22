#include "ParticleEmitter.h"

void ParticleEmitter::CreateData()
{
	// デバイス取得
	ID3D12Device* device = DirectXCommon::GetInstance()->GetDevice();

	emit_.CreateConstantBuffer(device);
	emit_.Mapping();

	perFrame_.CreateConstantBuffer(device);
	perFrame_.Mapping();
}
