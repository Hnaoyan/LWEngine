struct Camera
{
    float32_t4x4 view;
    float32_t4x4 projection;
    float32_t3 position;
};

struct TriangleVertex
{
    float32_t3 position;
    float32_t4 color;
    float32_t2 texcoord;
};

struct Material
{
    float32_t4 color;
    float32_t4x4 uvTransform;
};

struct VSOutput
{
    float32_t4 position : SV_Position;
    float32_t4 color : COLOR0;
    float32_t2 texcoord : TEXCOORD0;
};
