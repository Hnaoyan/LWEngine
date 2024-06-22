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

// 拡散反射
float32_t3 Diffuse(float32_t4 texColor, float cos)
{
    float32_t3 diffuse = gMaterial.color.rgb * texColor.rgb * gDirectionalLight.color.rgb * cos * gDirectionalLight.intensity;
    return diffuse;
}

// 鏡面反射
float32_t3 Specular(VSOutput input)
{
    // カメラへの方向
    float32_t3 toEye = normalize(gCamera.position - input.worldPosition);
    // 入射光の反射ベクトル
    float32_t3 reflectLight = reflect(gDirectionalLight.direction, normalize(input.normal));
    // 鏡面反射の強度
    float RdotE = dot(reflectLight, toEye);
    float specularPow = pow(saturate(RdotE), gMaterial.shininess);
    float32_t3 specular = gDirectionalLight.color.rgb * gDirectionalLight.intensity * specularPow * float32_t3(1.0f, 1.0f, 1.0f);
    return specular;
}

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
    //resultColor = gMaterial.color * textureColor * gDirectionalLight.color * cos * gDirectionalLight.intensity;

    float32_t3 diffuse = Diffuse(textureColor, cos);
    float32_t3 specular = Specular(input);
    
    resultColor.rgb = diffuse + specular;
    resultColor.a = gMaterial.color.a * textureColor.a;
    
    //resultColor = gMaterial.color * textureColor;
    
    output.color = resultColor;
    
    return output;

}	