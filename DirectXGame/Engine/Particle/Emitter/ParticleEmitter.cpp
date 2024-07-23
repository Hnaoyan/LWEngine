#include "ParticleEmitter.h"
#include "../GPUParticleSystem.h"

void ParticleEmitter::CreateData()
{
	// デバイス取得
	ID3D12Device* device = DirectXCommon::GetInstance()->GetDevice();

	particles_.CreateBuffer(device, GPUParticleSystem::kNumInstanceMax);
}
