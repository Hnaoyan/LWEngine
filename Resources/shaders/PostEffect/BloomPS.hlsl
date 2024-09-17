#include "FullScreen.hlsli"
#include "../Utility/HSV_RGB.hlsli"

Texture2D<float32_t4> gTexture : register(t0);
SamplerState gSampler : register(s0);

ConstantBuffer<HSVData> gHSV : register(b5);

struct PixelShaderOutput
{
    float32_t4 color : SV_TARGET0;
};

PixelShaderOutput main(VertexShaderOutput input)
{
    PixelShaderOutput output;
    float32_t4 textureColor = gTexture.Sample(gSampler, input.texcoord);
    
    float32_t3 hsv = RGBToHSV(textureColor.rgb);
    
    // 調整
    hsv.x += gHSV.hue;
    hsv.y += gHSV.saturation;
    hsv.z += gHSV.value;
 
    hsv.x = WrapValue(hsv.x, 0.0f, 1.0f);
    hsv.y = saturate(hsv.y);
    hsv.z = saturate(hsv.z);
    
    float32_t3 rgb = HSVToRGB(hsv);
    
    output.color.rgb = rgb;
    output.color.a = textureColor.a;
    return output;
}
