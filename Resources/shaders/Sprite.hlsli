struct spriteData
{
    float32_t4 color;
    float32_t4x4 World;
    float32_t4x4 WVP;
};

ConstantBuffer<spriteData> cBuffer : register(b0);

//cbuffer cbuff : register(b0)
//{
//    float32_t4 color;
//    float32_t4x4 mat;
//}

struct VSOutput
{
    float32_t4 svPosition : SV_POSITION;
    float32_t2 uv : TEXCOORD;
};
