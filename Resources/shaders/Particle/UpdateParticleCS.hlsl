#include "Particle.hlsli"
#include "ParticleCalc.hlsli"

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
            // Index管理
            if ((freeListIndex + 1) < kMaxParticle)
            {
                gFreeList[freeListIndex + 1] = particleIndex;
            }
            else
            {
                InterlockedAdd(gFreeListIndex[0], -1, freeListIndex);
            }
        }
        // 処理
        else
        {
            //---特有の処理---//
            // スケールの拡縮の処理
            // 2軸
            if (gParticle[particleIndex].isScaleDecrement == 1)
            {
                gParticle[particleIndex].scale.x -= (gParticle[particleIndex].currentTime / gParticle[particleIndex].lifetime);
                gParticle[particleIndex].scale.y -= (gParticle[particleIndex].currentTime / gParticle[particleIndex].lifetime);           
            }
            // 3軸
            else if (gParticle[particleIndex].isScaleDecrement == 2)
            {
                gParticle[particleIndex].scale.x -= (gParticle[particleIndex].currentTime / gParticle[particleIndex].lifetime);
                gParticle[particleIndex].scale.y -= (gParticle[particleIndex].currentTime / gParticle[particleIndex].lifetime);
                gParticle[particleIndex].scale.z -= (gParticle[particleIndex].currentTime / gParticle[particleIndex].lifetime);
            }
            // 重力を速度に計算するか
            if(gParticle[particleIndex].isGravity == 1)
            {
                float gravity = -3.5f;
                gParticle[particleIndex].velocity.y += (gravity) * (gParticle[particleIndex].currentTime / gParticle[particleIndex].lifetime);

            }
            
            //---共通の処理---//
            // 座標の処理
            gParticle[particleIndex].translate += gParticle[particleIndex].velocity;
            // 経過時間
            gParticle[particleIndex].currentTime += gPerFrame.deltaTime;
            // スケールの棄却処理
            gParticle[particleIndex].scale = ScaleCheck(gParticle[particleIndex].scale);
            // アルファ値の計算
            float32_t alpha = 0.0f;
            if (gParticle[particleIndex].isAlpha == 1)
            {
                alpha = 1.0f - (gParticle[particleIndex].currentTime / gParticle[particleIndex].lifetime);
            }
            else
            {
                alpha = gParticle[particleIndex].color.a - (gParticle[particleIndex].currentTime / gParticle[particleIndex].lifetime);
            }
            
            gParticle[particleIndex].color.a = saturate(alpha);    
        }
    }    
}