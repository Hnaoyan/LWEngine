#include "FullScreen.hlsli"

Texture2D<float32_t4> gTexture : register(t0);
SamplerState gSampler : register(s0);

ConstantBuffer<BlurData> gBlur : register(b2);

struct PixelShaderOutput
{
    float32_t4 color : SV_TARGET0;
};

PixelShaderOutput main(VertexShaderOutput input)
{
    PixelShaderOutput output;
    
    // 方向
    float32_t2 direction = input.texcoord - gBlur.center;
    // 色
    float32_t3 outputColor = float32_t3(0.0f, 0.0f, 0.0f);
    for (int32_t sampleIndex = 0; sampleIndex < gBlur.samplerNum; ++sampleIndex)
    {
        // サンプリング点を方向に進めながらサンプリングする
        float32_t2 texcoord = input.texcoord + direction * gBlur.blurWidth * float32_t(sampleIndex);
        outputColor.rgb += gTexture.Sample(gSampler, texcoord).rgb;
    }
    
    // 平均化
    outputColor.rgb *= rcp(gBlur.samplerNum);
    output.color.rgb = outputColor;
    output.color.a = 1.0f;
    //output.color = gTexture.Sample(gSampler, input.texcoord);
    return output;
}
