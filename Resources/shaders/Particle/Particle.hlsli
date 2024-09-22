#include "../Utility/Random.hlsli"

struct VertexShaderOutput
{
    float32_t4 position : SV_POSITION;
    float32_t3 normal : NORMAL0;
    float32_t2 texcoord : TEXCOORD0;
    float32_t4 color : COLOR0;
};

struct Material
{
    float32_t4 color;
    float32_t4x4 uvTransform;
    int32_t enableLighting;
    float32_t shininess;
    float32_t coefficient;
};

struct Camera
{
    float32_t4x4 view;
    float32_t4x4 projection;
    float32_t3 position;
};

struct Particle
{
    float32_t3 translate;
    float32_t3 scale;
    float32_t lifetime;
    float32_t3 velocity;
    float32_t currentTime;
    float32_t4 color;
    uint32_t isScaleDecrement;
    uint32_t isGravity;
};

struct PerView
{
    float32_t4x4 view;
    float32_t4x4 projection;
    float32_t4x4 billboardMatrix;
};

struct EmitterSphere
{
    float32_t3 translate;
    float32_t radius;
    uint32_t count;
    float32_t frequency;
    float32_t frequencyTime;
    uint32_t emit;
    uint32_t emitPattern;
};

struct PerFrame
{
    float32_t time;
    float32_t deltaTime;
};
