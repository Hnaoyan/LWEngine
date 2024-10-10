#include "Trail.hlsli"

struct PixelShaderOutput
{
    float32_t4 color : SV_TARGET0;
};

// マテリアル
ConstantBuffer<Material> gMaterial : register(b1);

Texture2D<float32_t4> tex : register(t0);
SamplerState smp : register(s0);

PixelShaderOutput main(VSOutput input)
{
    PixelShaderOutput output;
    float32_t4 transformedUV = mul(float32_t4(input.texcoord, 0.0f, 1.0f), gMaterial.uvTransform);
    output.color = tex.Sample(smp, transformedUV.xy) * input.color;
    if (output.color.a <= 0.01f)
    {
        discard;
    }
    
    return output;
}