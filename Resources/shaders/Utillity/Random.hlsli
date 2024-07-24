
float rand2dTo1d(float2 value, float2 dotDir = float2(12.9898, 78.233))
{
    float2 smallValue = sin(value);
    float random = dot(smallValue, dotDir);
    random = frac(sin(random) * 143758.5453);
    return random;
}

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

float2 rand3dTo2d(float3 value)
{
    return float2(
        rand3dTo1d(value, float3(12.989, 78.233, 37.719)),
        rand3dTo1d(value, float3(39.346, 11.135, 83.155))
    );
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
    
    float32_t2 Generate2D()
    {
        float32_t2 result = rand3dTo2d(seed);
        seed.xy = result;
        return result;
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
    
    float32_t2 GenerateRange2D(float32_t2 min, float32_t2 max)
    {
        float32_t2 result = Generate2D();
        return min + result * (max - min);
    }
  
    float32_t3 GenerateRange3D(float32_t3 min, float32_t3 max)
    {
        float32_t3 result = Generate3D();
        return min + result * (max - min);
    }
    
};
