#include "Particle.h"
#include "Engine/Base/DirectXCommon.h"
void Particle::CreateCSResource()
{
	ID3D12Device* device = DirectXCommon::GetInstance()->GetDevice();
	
	particleResources_ = DxCreateLib::ResourceLib::CreateResourceUAV(device, sizeof(ParticleCS) * 1024);

}
