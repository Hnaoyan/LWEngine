#include "Skybox.hlsli"

TextureCube<float32_t4> gTexture : register(t0);
SamplerState gSampler : register(s0);

ConstantBuffer<Material> gMaterial : register(b2);
//ConstantBuffer<DirectionalLight> gLight : register(b1);

// マテリアル
//ConstantBuffer<Material> gMaterial : register(t0);

// 出力
struct PixelShaderOutput
{
    float32_t4 color : SV_TARGET0;
};

PixelShaderOutput main(VSOutput input)
{
    PixelShaderOutput output;
    // サンプラーとTexCoord
    float32_t4 textureColor = gTexture.Sample(gSampler, input.texcoord);
    //output.color = textureColor;
    output.color = gMaterial.color * textureColor;
    
    return output;

}
