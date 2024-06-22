#include "Sprite.hlsli"

Texture2D<float32_t4> tex : register(t0);
SamplerState smp : register(s0);

float32_t4 main(VSOutput input) : SV_TARGET
{
    float4 transformedUV = mul(float32_t4(input.uv, 0.0f, 1.0f), cBuffer.UvTransform);
    float32_t4 textureColor = tex.Sample(smp, transformedUV.xy) * cBuffer.color;
    //float32_t4 textureColor = tex.Sample(smp, input.uv) * cBuffer.color;
    if (textureColor.a <= 0.01f)
    {
        discard;
    }
    return textureColor;
}
