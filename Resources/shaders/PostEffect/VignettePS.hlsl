#include "FullScreen.hlsli"

Texture2D<float32_t4> gTexture : register(t0);
SamplerState gSampler : register(s0);

ConstantBuffer<VignetteData> gVignette : register(b1);

struct PixelShaderOutput
{
    float32_t4 color : SV_TARGET0;
};

///
/// ビネット
///
PixelShaderOutput main(VertexShaderOutput input)
{
    PixelShaderOutput output;
    output.color = gTexture.Sample(gSampler, input.texcoord);

    // 
    float32_t2 correct = input.texcoord * (1.0f - input.texcoord.yx);
    
    float vignette = correct.x * correct.y * gVignette.scale;
    
    vignette = saturate(pow(vignette, gVignette.powValue));
    
    // ビネットの色
    float32_t3 vignetteColor = gVignette.color;
    
    float32_t3 vignetteEffect = (1.0 - vignette) * vignetteColor + vignette * output.color.rgb;

    output.color.rgb = vignetteEffect;
    
    return output;
}
