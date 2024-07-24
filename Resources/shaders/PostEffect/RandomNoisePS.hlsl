#include "FullScreen.hlsli"
#include "../Utillity/Random.hlsli"

Texture2D<float32_t4> gTexture : register(t0);
SamplerState gSampler : register(s0);

ConstantBuffer<RandomNoiseData> gNoise : register(b4);

struct PixelShaderOutput
{
    float32_t4 color : SV_TARGET0;
};

PixelShaderOutput main(VertexShaderOutput input)
{
    PixelShaderOutput output;
    // 乱数生成
    float32_t random = rand2dTo1d(input.texcoord * gNoise.time);
    // 色にする
    if (gNoise.enable == 0)
    {
        output.color = float32_t4(random, random, random, 1.0f);
    }
    else
    {
        output.color = gTexture.Sample(gSampler, input.texcoord);
        output.color *= float32_t4(random, random, random, 1.0f);
    }
    return output;
}
