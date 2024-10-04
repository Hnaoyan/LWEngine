#include "../Particle.hlsli"

///
/// Default
///
Particle DefaultPop(RandomGenerator generator)
{
    Particle particle = (Particle) 0;
    particle.scale = generator.GenerateRange3D(float32_t3(0.5f, 0.5f, 0.5f), float32_t3(1.0f, 1.0f, 1.0f));
    //float32_t value = 0.5f;
    float32_t3 min = { -0.5f, -0.5f, -0.5f };
    float32_t3 max = { 0.5f, 0.5f, 0.5f };
    particle.translate = generator.GenerateRange3D(min, max);
    particle.translate *= 3.0f;
    particle.color.rgb = generator.Generate3D();
    particle.color.a = 1.0f;
    particle.lifetime = 1.0f;
    particle.currentTime = 0.0f;
    return particle;
}

///
/// PlayerMove
/// 
Particle PlayerMove(RandomGenerator generator,float32_t3 emitterPosition)
{
    Particle particle = (Particle) 0;
    float32_t3 min = { 0.3f, 0.3f, 1.0f };
    float32_t3 max = { 0.55f, 0.55f, 1.0f };
    particle.scale = generator.GenerateRange3D(min, max);
    particle.translate = emitterPosition;
    particle.color.rgb = float32_t3(0.0f, 0.01f, 0.05f);
    particle.color.a = 0.75f;
    particle.lifetime = 1.0f;
    particle.currentTime = 0.0f;
    particle.isAlpha = 1;
    return particle;
}

Particle BossBullet(RandomGenerator generator, float32_t3 emitterPosition)
{
    Particle particle = (Particle) 0;
    float32_t3 min = { 1.0f, 1.0f, 1.0f };
    float32_t3 max = { 1.15f, 1.15f, 1.0f };
    particle.scale = generator.GenerateRange3D(min, max);
    particle.translate = emitterPosition;
    particle.color.rgb = float32_t3(1.0f, 1.0f, 1.0f);
    particle.color.a = 0.85f;
    particle.lifetime = 1.0f;
    particle.currentTime = 0.0f;
    particle.isAlpha = 1;
    particle.isScaleDecrement = 1;
    return particle;
}

Particle BossDamage(RandomGenerator generator, float32_t3 emitterPosition)
{
    Particle particle = (Particle) 0;
    float32_t maxSize = 3.0f;
    float32_t minSize = 1.5f;
    float32_t randScale = generator.GenerateRange1D(minSize, maxSize);
    particle.scale = float32_t3(randScale, randScale, randScale);
    float32_t value = 3.0f;
    float32_t3 min = { -value, -value, -value };
    float32_t3 max = { value, value, value };
    
    float32_t3 test = generator.Generate3D();
    particle.translate = generator.GenerateRange3D(min, max);
    
    float32_t3 direct = particle.translate;
    direct = normalize(direct);
    particle.velocity = direct;
   
    particle.translate *= 3.0f;
    particle.translate += emitterPosition;
    particle.color.rgb = generator.GenerateRange3D(float32_t3(0.0f, 0.95f, 0.95f), float32_t3(0.05f, 1.0f, 1.0f));
    particle.color.a = generator.GenerateRange1D(float32_t(0.65f), float32_t(0.85f));
    particle.lifetime = 6.0f;
    particle.currentTime = 0.0f;
    particle.isScaleDecrement = 2;
    particle.isGravity = 1;
    
    return particle;
}
