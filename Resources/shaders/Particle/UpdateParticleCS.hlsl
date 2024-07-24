#include "Particle.hlsli"

static const uint32_t kMaxParticle = 1024;

RWStructuredBuffer<Particle> gParticle : register(u0);
RWStructuredBuffer<int32_t> gFreeListIndex : register(u1);
RWStructuredBuffer<uint32_t> gFreeList : register(u2);
ConstantBuffer<PerFrame> gPerFrame : register(b1);

[numthreads(1024, 1, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
    uint32_t particleIndex = DTid.x;
    
    if (particleIndex < kMaxParticle)
    {        
        // Alpha値が0になっているのをFreeとする
        if (gParticle[particleIndex].color.a == 0.0f)
        {
            // スケールに0を入れてVSで棄却されるように
            gParticle[particleIndex].scale = float32_t3(0.0f, 0.0f, 0.0f);
            int32_t freeListIndex;
            InterlockedAdd(gFreeListIndex[0], 1, freeListIndex);
            
            if ((freeListIndex + 1) < kMaxParticle)
            {
                gFreeList[freeListIndex + 1] = particleIndex;
            }
            else
            {
                InterlockedAdd(gFreeListIndex[0], -1, freeListIndex);
            }
        }
        else
        {
            gParticle[particleIndex].translate += gParticle[particleIndex].velocity;
            gParticle[particleIndex].currentTime += gPerFrame.deltaTime;
            
            float32_t alpha = 1.0f - (gParticle[particleIndex].currentTime / gParticle[particleIndex].lifetime);
            gParticle[particleIndex].color.a = saturate(alpha);
        }
    }
    
}