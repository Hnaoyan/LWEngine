#include "../Particle.hlsli"

ConstantBuffer<EmitterSphere> gEmitter : register(b0);
ConstantBuffer<PerFrame> gPerFrame : register(b1);
RWStructuredBuffer<Particle> gParticle : register(u0);

[numthreads(1, 1, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
    if (gEmitter.emit != 0)
    {
        RandomGenerator generator;
        generator.seed = (DTid + gPerFrame.time) * gPerFrame.time;
        
        for (uint32_t countIndex = 0; countIndex < gEmitter.count; ++countIndex)
        {
            // 射出
            gParticle[countIndex].scale = generator.GenerateRange3D(float32_t3(0.5f, 0.5f, 0.5f), float32_t3(1.0f, 1.0f, 1.0f));
            float32_t value = 0.5f;
            gParticle[countIndex].translate = generator.GenerateRange3D(float32_t3(-value, -value, -value), float32_t3(value, value, value));
            gParticle[countIndex].translate *= 3.0f;
            gParticle[countIndex].color.rgb = generator.Generate3D();
            gParticle[countIndex].color.a = 1.0f;
            //gParticle[countIndex].scale = float32_t3(0.5f, 0.5f, 0.5f);
            //gParticle[countIndex].translate = float32_t3(0.0f, 0.0f, 0.0f);
            //gParticle[countIndex].color = float32_t4(1.0f, 1.0f, 1.0f, 1.0f);

        }
    }  
}