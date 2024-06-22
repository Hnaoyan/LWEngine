struct WorldTransform
{
    float32_t4x4 worldMat;
    float32_t4x4 WorldInverseTranspose;
};

struct Camera
{
    float32_t4x4 view;
    float32_t4x4 projection;
    float32_t3 position;
};

struct Material
{
    float32_t4 color;
    float32_t4x4 uvTransform;
    int32_t enableLighting;
    float32_t shininess;
};

struct DirectionalLight
{
    float32_t4 color;   // 
    float32_t3 direction;   // 
    float intensity;    // 
};

struct PointLight
{
    float32_t4 color;   // 
    float32_t3 position;    // 
    float intensity;    // 
    float radius;   // 
    float decay;    // 
};

struct SpotLight
{
    float32_t4 color; // 
    float32_t3 position; // 
    float intensity; // 
    float radius; // 
    float decay; // 
};

struct VSOutput
{
    float32_t4 position : SV_POSITION;
    float32_t3 normal : NORMAL0;
    float32_t2 texcoord : TEXCOORD0;
    float32_t3 worldPosition : POSITION0;
};

struct Well
{
    float32_t4x4 skeletonSpaceMatrix;
    float32_t4x4 skeletonSpaceInverseTransposeMatrix;
};