#include "3DTriangle.hlsli"

struct PixelShaderOutput
{
    float32_t4 color : SV_TARGET0;
};

Texture2D<float32_t4> tex : register(t0);
SamplerState smp : register(s0);

PixelShaderOutput main(VSOutput input)
{
    PixelShaderOutput output;
    //output.color = input.color;
    
    output.color = tex.Sample(smp, input.texcoord) * input.color;
    if (output.color.a <= 0.01f)
    {
        discard;
    }
    
    return output;
}