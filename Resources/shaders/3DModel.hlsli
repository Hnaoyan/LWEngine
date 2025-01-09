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
    float32_t coefficient;
    float32_t3 dissolveColor;
    float32_t dissolveThreshold;
    float32_t discardThreshold;
};

struct DirectionalLight
{
    float32_t4 color;   // 
    float32_t3 direction;   // 
    float32_t intensity; // 
};

struct PointLight
{
    float32_t4 color;   // 
    float32_t3 position;    // 
    float32_t intensity; // 
    float32_t radius; // 
    float32_t decay; // 
};

struct SpotLight
{
    float32_t4 color; // 
    float32_t3 position; // 
    float32_t intensity; // 
    float32_t3 direction;
    float32_t distance;
    float32_t decay;
    float32_t cosAngle;
    float32_t cosFalloffStart;
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

struct Vertex
{
    float32_t4 position;
    float32_t3 normal;
    float32_t2 texcoord;
};

struct VertexInfluence
{
    float32_t4 weight;
    int32_t4 index;
};

struct SkinningInformation
{
    uint32_t numVertices;
};
