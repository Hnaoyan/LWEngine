#include "Sprite.hlsli"

struct VSInput
{
    float32_t4 position : POSITION0;
    float32_t2 texcoord : TEXCOORD0;
    //float32_t3 normal : NORMAL0;
};

VSOutput main(VSInput input)
{
    VSOutput output;
    output.svPosition = mul(input.position, mul(cBuffer.WVP, cBuffer.World));
    output.uv = input.texcoord;
    return output;
}