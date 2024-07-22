#include "Particle.hlsli"

static const uint32_t kMaxParticle = 1024;

RWStructuredBuffer<Particle> gParticle : register(u0);
RWStructuredBuffer<int32_t> gFreeCounter : register(u1);

[numthreads(1024, 1, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
    uint32_t particleIndex = DTid.x;
    
    if (particleIndex == 0)
    {
        gFreeCounter[0] = 0;
    }
    
    if (particleIndex < kMaxParticle)
    {
        // Particle構造体の全要素を0で埋める
        gParticle[particleIndex] = (Particle) 0;
        gParticle[particleIndex].scale = float32_t3(0.5f, 0.5f, 0.5f);
        gParticle[particleIndex].color = float32_t4(1.0f, 1.0f, 1.0f, 1.0f);

    }
    
}