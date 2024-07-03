#include "FullScreen.hlsli"

Texture2D<float32_t4> gTexture : register(t0);
Texture2D<float32_t> gMaskTexture : register(t1);
SamplerState gSampler : register(s0);

ConstantBuffer<DissolveData> gDissolve : register(b3);

struct PixelShaderOutput
{
    float32_t4 color : SV_TARGET0;
};

PixelShaderOutput main(VertexShaderOutput input)
{
    PixelShaderOutput output;
    //float32_t3 edgeColor = { 1.0f, 0.4f, 0.3f };
    //float32_t threshold = 0.5f;
    
    float32_t mask = gMaskTexture.Sample(gSampler, input.texcoord);
    // maskの値が閾値以下はdiscard
    if (mask <= gDissolve.threshold)
    {
        discard;
    }
    // 色描画
    output.color = gTexture.Sample(gSampler, input.texcoord);
    
    // Edgeっぽさを算出
    float32_t edge = 1.0f - smoothstep(0.5f, -0.53f, mask);
    // Edgeっぽいほど指定した色を加算
    output.color.rgb += edge * gDissolve.color;
    return output;
}
