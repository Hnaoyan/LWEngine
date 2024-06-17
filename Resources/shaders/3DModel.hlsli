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
    int32_t enableLighting;
};

struct DirectionalLight
{
    float32_t4 color;
    float32_t3 direction;
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