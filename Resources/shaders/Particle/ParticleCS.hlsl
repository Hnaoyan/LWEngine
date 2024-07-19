#include "Particle.hlsli"

static const uint32_t kMaxParticle = 1024;

RWStructuredBuffer<Particle> gParticle : register(u0);

[numthreads(1024, 1, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
    uint32_t particleIndex = DTid.x;
    
    if (particleIndex < kMaxParticle)
    {
        // Particle構造体の全要素を0で埋める
        gParticle[particleIndex] = (Particle) 0;
    }
    
}