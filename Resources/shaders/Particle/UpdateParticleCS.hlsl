#include "Particle.hlsli"
#include "ParticleCalc.hlsli"

static const uint32_t kMaxParticle = 1024;

RWStructuredBuffer<Particle> gParticle : register(u0);
RWStructuredBuffer<int32_t> gFreeListIndex : register(u1);
RWStructuredBuffer<uint32_t> gFreeList : register(u2);
ConstantBuffer<PerFrame> gPerFrame : register(b1);

void ParticleDelete(uint32_t index)
{
    // スケールに0を入れてVSで棄却されるように
    gParticle[index].scale = float32_t3(0.0f, 0.0f, 0.0f);
    int32_t freeListIndex;
    InterlockedAdd(gFreeListIndex[0], 1, freeListIndex);
            // Index管理
    if ((freeListIndex + 1) < kMaxParticle)
    {
        gFreeList[freeListIndex + 1] = index;
    }
    else
    {
        InterlockedAdd(gFreeListIndex[0], -1, freeListIndex);
    }
}

[numthreads(1024, 1, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
    uint32_t particleIndex = DTid.x;
    
    if (particleIndex < kMaxParticle)
    {        
        // Alpha値が0になっているのをFreeとする
        if (gParticle[particleIndex].color.a == 0.0f)
        {
            ParticleDelete(particleIndex);
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
            else if (gParticle[particleIndex].isScaleDecrement == 3)
            {
                gParticle[particleIndex].scale.xy = Easing(float32_t2(0.7f,0.7f), float32_t2(0.0f,0.0f), (gParticle[particleIndex].currentTime / gParticle[particleIndex].lifetime));
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
            // 通常の減産
            if(gParticle[particleIndex].isAlpha == 0)
            {
                alpha = gParticle[particleIndex].color.a - (gParticle[particleIndex].currentTime / gParticle[particleIndex].lifetime);
            }
            // 1.0である場合のやつ
            else if (gParticle[particleIndex].isAlpha == 1)
            {
                alpha = 1.0f - (gParticle[particleIndex].currentTime / gParticle[particleIndex].lifetime);
            }
            // 減少しないやつ
            else if (gParticle[particleIndex].isAlpha == 2)
            {
                alpha = gParticle[particleIndex].color.a;
            }
            else if (gParticle[particleIndex].isAlpha == 3)
            {
                alpha = Easing(gParticle[particleIndex].startAlpha, 0.0f, (gParticle[particleIndex].currentTime / gParticle[particleIndex].lifetime));
            }
            else
            {
                alpha = gParticle[particleIndex].color.a;
            }
            // アルファの設定
            gParticle[particleIndex].color.a = alpha;
            
            // ライフタイムで削除処理
            if (gParticle[particleIndex].currentTime > gParticle[particleIndex].lifetime)
            {
                ParticleDelete(particleIndex);
            }
            
            //gParticle[particleIndex].color.a = saturate(alpha);    
        }
    }    
}