struct Camera
{
    float32_t4x4 view;
    float32_t4x4 projection;
    float32_t3 position;
};

struct LineVertex
{
    float32_t4 position;
    float32_t4 color;
};

struct VSOutput
{
    float32_t4 position : SV_Position;
    float32_t4 color : COLOR0;
};
