#include "FullScreen.hlsli"
#include "../Utility/PostCalc.hlsli"

Texture2D<float32_t4> gTexture : register(t0);
SamplerState gSampler : register(s0);

ConstantBuffer<GrayscaleData> gGrayscale : register(b7);

struct PixelShaderOutput
{
    float32_t4 color : SV_TARGET0;
};

PixelShaderOutput main(VertexShaderOutput input)
{
    PixelShaderOutput output;
    output.color = gTexture.Sample(gSampler, input.texcoord);
    float32_t3 originalColor = output.color.rgb;

    float32_t value = dot(output.color.rgb, float32_t3(0.2125f, 0.7154f, 0.0721f));
    float32_t3 grayscaleColor = float32_t3(value, value, value);
    output.color.rgb = Easing(originalColor, grayscaleColor, gGrayscale.lerpFactor);
    return output;
}
