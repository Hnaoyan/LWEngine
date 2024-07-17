struct VertexShaderOutput
{
    float32_t4 position : SV_POSITION;
    float32_t2 texcoord : TEXCOORD0;
};

struct BlurData
{
    float32_t2 center;
    float32_t blurWidth;
    int32_t samplerNum;
};

struct VignetteData
{
    float32_t3 color;
    float32_t scale;
    float32_t powValue;
};

struct DissolveData
{
    float32_t3 color;
    float32_t threshold;
};

struct RandomNoiseData
{
    int32_t enable;
    float32_t time;
};

struct HSVData
{
    float32_t hue;
    float32_t saturation;
    float32_t value;
};
