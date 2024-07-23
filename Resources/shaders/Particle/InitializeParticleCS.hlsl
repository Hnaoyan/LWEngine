#include "Particle.hlsli"

static const uint32_t kMaxParticle = 1024;

RWStructuredBuffer<Particle> gParticle : register(u0);
RWStructuredBuffer<int32_t> gFreeListIndex : register(u1);
RWStructuredBuffer<uint32_t> gFreeList : register(u2);

[numthreads(1024, 1, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
    uint32_t particleIndex = DTid.x;
    
    if (particleIndex < kMaxParticle)
    {
        // Particle構造体の全要素を0で埋める
        gParticle[particleIndex] = (Particle) 0;
        gFreeList[particleIndex] = particleIndex;
    }
   if (particleIndex == 0)
    {
        gFreeListIndex[0] = kMaxParticle - 1;
    }
}