#include "3DModel.hlsli"

Texture2D<float32_t4> gTexture : register(t0);
SamplerState gSampler : register(s0);

ConstantBuffer<Camera> gCamera : register(b0);
ConstantBuffer<Material> gMaterial : register(b1);
ConstantBuffer<DirectionalLight> gDirectionalLight : register(b2);

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
    float4 transformedUV = mul(float32_t4(input.texcoord, 0.0f, 1.0f), gMaterial.uvTransform);
    float32_t4 textureColor = gTexture.Sample(gSampler, transformedUV.xy);
    //float32_t4 textureColor = gTexture.Sample(gSampler, input.texcoord);
    float32_t4 resultColor;
    float NdotL = dot(normalize(input.normal), -gDirectionalLight.direction);
    float cos = pow(NdotL * 0.5f + 0.5f, 2.0f);
    resultColor = gMaterial.color * textureColor * gDirectionalLight.color * cos * gDirectionalLight.intensity;
    //float cos = saturate(dot(normalize(input.normal), -gDirectionalLight.direction));
    //resultColor = gMaterial.color * textureColor * gDirectionalLight.color * cos * gDirectionalLight.intensity;

    //resultColor = gMaterial.color * textureColor;
    
    output.color = resultColor;
    
    return output;

}	