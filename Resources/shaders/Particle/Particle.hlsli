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
};

struct PerFrame
{
    float32_t time;
    float32_t deltaTime;
};

float rand3dTo1d(float3 value, float3 dotDir = float3(12.9898, 78.233, 37.719))
{
    //make value smaller to avoid artefacts
    float3 smallValue = sin(value);
    //get scalar value from 3d vector
    float random = dot(smallValue, dotDir);
    //make value more random by making it bigger and then taking the factional part
    random = frac(sin(random) * 143758.5453);
    return random;
}

float3 rand3dTo3d(float3 value)
{
    return float3(
        rand3dTo1d(value, float3(12.989, 78.233, 37.719)),
        rand3dTo1d(value, float3(39.346, 11.135, 83.155)),
        rand3dTo1d(value, float3(73.156, 52.235, 09.151))
    );
}

class RandomGenerator
{
    float32_t3 seed;
    float32_t3 Generate3D()
    {
        seed = rand3dTo3d(seed);
        return seed;
    }
    
    float32_t Generate1D()
    {
        float32_t result = rand3dTo1d(seed);
        seed.x = result;
        return result;
    }
    
    float32_t GenerateRange1D(float32_t min, float32_t max)
    {
        float32_t1 result = Generate1D();
        return min + result * (max - min);
    }
    
    float32_t3 GenerateRange3D(float32_t3 min, float32_t3 max)
    {
        float32_t3 result = Generate3D();
        return min + result * (max - min);
    }
    
};
