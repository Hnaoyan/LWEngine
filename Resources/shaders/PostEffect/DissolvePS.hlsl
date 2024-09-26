#include "FullScreen.hlsli"
#include "../Utility/PostCalc.hlsli"

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
    
    // マスクの処理
    float32_t mask = gMaskTexture.Sample(gSampler, input.texcoord);
    ProcessMask(mask, gDissolve.threshold);
    
    // 色描画
    output.color = gTexture.Sample(gSampler, input.texcoord);
    
    // Edgeっぽいほど指定した色を加算
    float32_t3 addColor = Dissolve(gDissolve.color, gDissolve.threshold, mask);
    output.color.rgb += addColor;
    return output;
}
