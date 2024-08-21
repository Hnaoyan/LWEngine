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
Particle UnNormal(RandomGenerator generator,float32_t3 emitterPosition)
{
    Particle particle = (Particle) 0;
    float32_t3 min = { 0.5f, 0.5f, 1.0f };
    float32_t3 max = { 1.0f, 1.0f, 1.0f };
    particle.scale = generator.GenerateRange3D(min, max);
    particle.translate = emitterPosition;
    particle.color.rgb = float32_t3(1.0f, 1.0f, 1.0f);
    particle.color.a = 1.0f;
    particle.lifetime = 1.0f;
    particle.currentTime = 0.0f;
    return particle;
}

Particle BossBullet(RandomGenerator generator, float32_t3 emitterPosition)
{
    Particle particle = (Particle) 0;
    float32_t3 min = { 0.5f, 0.5f, 1.0f };
    float32_t3 max = { 2.0f, 2.0f, 1.0f };
    particle.scale = generator.GenerateRange3D(min, max);
    particle.translate = emitterPosition;
    particle.color.rgb = float32_t3(1.0f, 1.0f, 1.0f);
    particle.color.a = 1.0f;
    particle.lifetime = 0.3f;
    particle.currentTime = 0.0f;
    return particle;
}

Particle BossDamage(RandomGenerator generator, float32_t3 emitterPosition)
{
    Particle particle = (Particle) 0;
    particle.scale = generator.GenerateRange3D(float32_t3(0.5f, 0.5f, 0.5f), float32_t3(1.0f, 1.0f, 1.0f));
    //float32_t value = 0.5f;
    float32_t3 min = { -0.5f, -0.5f, -0.5f };
    float32_t3 max = { 0.5f, 0.5f, 0.5f };
    particle.translate = generator.GenerateRange3D(min, max);
    
    float32_t3 direct = particle.translate - emitterPosition;
    direct = normalize(direct);
    particle.velocity = direct * 1.5f;
   
    particle.translate *= 3.0f;
    particle.translate += emitterPosition;
    particle.color.rgb = generator.Generate3D();
    particle.color.a = 1.0f;
    particle.lifetime = 1.0f;
    particle.currentTime = 0.0f;
    
    return particle;
}
