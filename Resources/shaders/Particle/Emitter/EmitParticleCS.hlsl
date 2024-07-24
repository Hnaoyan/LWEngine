#include "../Particle.hlsli"

static const uint32_t kMaxParticle = 1024;

ConstantBuffer<EmitterSphere> gEmitter : register(b0);
ConstantBuffer<PerFrame> gPerFrame : register(b1);
RWStructuredBuffer<Particle> gParticle : register(u0);
RWStructuredBuffer<int32_t> gFreeListIndex : register(u1);
RWStructuredBuffer<uint32_t> gFreeList : register(u2);

Particle DefaultInitialize(Particle data, float32_t3 seed, uint32_t index)
{
    Particle particle = data;  
    RandomGenerator generator;
    generator.seed = seed;
    particle.scale = generator.GenerateRange3D(float32_t3(0.5f, 0.5f, 0.5f), float32_t3(0.5f * 2.0f, 0.5f * 2.0f, 0.5f * 2.0f));
    float32_t value = 0.5f;
    particle.translate = generator.GenerateRange3D(float32_t3(-value, -value, -value), float32_t3(value, value, value));
    particle.translate *= 3.0f;
    particle.color.rgb = generator.Generate3D();
    particle.color.a = 1.0f;
    particle.lifetime = 1.0f;
    particle.currentTime = 0.0f;

    return particle;
}

[numthreads(1, 1, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
    if (gEmitter.emit != 0)
    {
        RandomGenerator generator;
        generator.seed = (DTid + gPerFrame.time) * gPerFrame.time;
        
        for (uint32_t countIndex = 0; countIndex < gEmitter.count; ++countIndex)
        {
            int32_t freeListIndex;
            // リストの一個前を設定してIndexを取得
            InterlockedAdd(gFreeListIndex[0], -1, freeListIndex);
            // 空いている場合
            if (0 <= freeListIndex && freeListIndex < kMaxParticle)
            {
                uint32_t particleIndex = gFreeList[freeListIndex];
                // 初期化処理
                //gParticle[particleIndex] = DefaultInitialize(gParticle[particleIndex], generator.seed, particleIndex);
                gParticle[particleIndex].scale = generator.GenerateRange3D(float32_t3(0.5f, 0.5f, 0.5f), float32_t3(1.0f, 1.0f, 1.0f));
                float32_t value = 0.5f;
                gParticle[particleIndex].translate = generator.GenerateRange3D(float32_t3(-value, -value, -value), float32_t3(value, value, value));
                gParticle[particleIndex].translate *= 3.0f;
                gParticle[particleIndex].translate += gEmitter.translate;
                gParticle[particleIndex].color.rgb = generator.Generate3D();
                gParticle[particleIndex].color.a = 1.0f;
                gParticle[particleIndex].lifetime = 1.0f;
                gParticle[particleIndex].currentTime = 0.0f;
            }
            // 空いていない場合
            else
            {
                // 発生させられなかったので、減らしてしまった分もとに戻す。
                InterlockedAdd(gFreeListIndex[0], 1);
                // Emit中にParticleは消えないので、この後発生することはないため終了
                break;
            }
        }
    }  
}
