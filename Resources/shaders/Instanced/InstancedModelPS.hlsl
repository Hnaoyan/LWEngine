#include "Instanced.hlsli"

Texture2D<float32_t4> gTexture : register(t0);
TextureCube<float32_t4> gEnvironmentTexture : register(t1);
SamplerState gSampler : register(s0);
// カメラ
ConstantBuffer<Camera> gCamera : register(b0);
// マテリアル
ConstantBuffer<Material> gMaterial : register(b1);
// ライティング
ConstantBuffer<DirectionalLight> gDirectionalLight : register(b2);
ConstantBuffer<PointLight> gPointLight : register(b3);
ConstantBuffer<SpotLight> gSpotLight : register(b4);

// 出力
struct PixelShaderOutput
{
    float32_t4 color : SV_TARGET0;
};

struct LightingCalcValue
{
    // 方向
    float32_t3 direct;
    
    // 鏡面反射
    float32_t3 halfVector;
    float32_t NDotH;
    float32_t SpecularPow;
    float32_t3 specular;
    
    // 拡散反射
    float32_t NDotL;
    float32_t Cos;
    float32_t3 diffuse;
};

///
/// ブリンフォン反射
///
float32_t4 BlinnPhongReflection(VSOutput input, float32_t4 texColor, float32_t3 toEye)
{
    // 返り値
    float32_t4 resultColor;
    ///---平行光源---//
    //---鏡面反射---//
    LightingCalcValue directionalValue;
    directionalValue.halfVector = normalize(-gDirectionalLight.direction + toEye);
    directionalValue.NDotH = dot(normalize(input.normal), directionalValue.halfVector);
    directionalValue.SpecularPow = pow(saturate(directionalValue.NDotH), gMaterial.shininess);
    directionalValue.specular = gDirectionalLight.color.rgb * gDirectionalLight.intensity * directionalValue.SpecularPow * float32_t3(1.0f, 1.0f, 1.0f);
    //---拡散反射---//
    directionalValue.NDotL = dot(normalize(input.normal), -gDirectionalLight.direction);
    directionalValue.Cos = pow(directionalValue.NDotL * 0.5f + 0.5f, 2.0f);
    directionalValue.diffuse = gMaterial.color.rgb * texColor.rgb * gDirectionalLight.color.rgb * directionalValue.Cos * gDirectionalLight.intensity;
    
    ///---点光源---//
    LightingCalcValue pointLightValue;
    // 方向
    pointLightValue.direct = normalize(input.worldPosition - gPointLight.position);
    // 減衰係数
    float32_t distance = length(gPointLight.position - input.worldPosition);
    float32_t factor = pow(saturate(-distance / gPointLight.radius + 1.0), gPointLight.decay);

    //---鏡面反射---//
    pointLightValue.halfVector = normalize(pointLightValue.direct + toEye);
    pointLightValue.NDotH = dot(normalize(input.normal), pointLightValue.halfVector);
    pointLightValue.SpecularPow = pow(saturate(pointLightValue.NDotH), gMaterial.shininess);
    pointLightValue.specular = gPointLight.color.rgb * gPointLight.intensity * pointLightValue.SpecularPow * float32_t3(1.0f, 1.0f, 1.0f) * factor;
    //---拡散反射---//
    pointLightValue.NDotL = dot(normalize(input.normal), -pointLightValue.direct);
    pointLightValue.Cos = pow(pointLightValue.NDotL * 0.5f + 0.5f, 2.0f);
    pointLightValue.diffuse = gMaterial.color.rgb * texColor.rgb * gPointLight.color.rgb * pointLightValue.Cos * gPointLight.intensity * factor;
    
    ///---照射光源---//
    LightingCalcValue spotLightValue;
    // 入射光
    float32_t3 spotLightDirectionOnSurface = normalize(input.worldPosition - gSpotLight.position);
    // 角度
    float32_t cosAngle = dot(spotLightDirectionOnSurface, gSpotLight.direction);
    float32_t falloffFactor;
    if (gSpotLight.cosFalloffStart == 0.0f)
    {
        falloffFactor = saturate((cosAngle - gSpotLight.cosAngle) / (1.0f - gSpotLight.cosAngle));
    }
    else
    {
        falloffFactor = saturate((cosAngle - gSpotLight.cosAngle) / ((gSpotLight.cosFalloffStart + gSpotLight.cosAngle) - gSpotLight.cosAngle));
    }
    float32_t spotDistance = length(gSpotLight.position - input.worldPosition);
    float32_t attenuationFactor = pow(saturate(-spotDistance / gSpotLight.distance + 1.0), gSpotLight.decay);

    //---鏡面反射---//
    spotLightValue.halfVector = normalize(gSpotLight.direction + toEye);
    spotLightValue.NDotH = dot(normalize(input.normal), spotLightValue.halfVector);
    spotLightValue.SpecularPow = pow(saturate(spotLightValue.NDotH), gMaterial.shininess);
    spotLightValue.specular = gSpotLight.color.rgb * gSpotLight.intensity * spotLightValue.SpecularPow * float32_t3(1.0f, 1.0f, 1.0f) * falloffFactor * attenuationFactor;
    //---拡散反射---//
    spotLightValue.NDotL = dot(normalize(input.normal), -gSpotLight.direction);
    spotLightValue.Cos = pow(spotLightValue.NDotL * 0.5f + 0.5f, 2.0f);
    spotLightValue.diffuse = gMaterial.color.rgb * texColor.rgb * gSpotLight.color.rgb * spotLightValue.Cos * gSpotLight.intensity * falloffFactor * attenuationFactor;
    
    // 結果
    resultColor.rgb = directionalValue.diffuse + directionalValue.specular + pointLightValue.diffuse + pointLightValue.specular + spotLightValue.diffuse + spotLightValue.specular;
    resultColor.a = gMaterial.color.a * texColor.a;
    return resultColor;
}

PixelShaderOutput main(VSOutput input)
{
    PixelShaderOutput output;
    // サンプラーとTexCoord
    float4 transformedUV = mul(float32_t4(input.texcoord, 0.0f, 1.0f), gMaterial.uvTransform);
    float32_t4 textureColor = gTexture.Sample(gSampler, transformedUV.xy);
    // カメラへの方向
    float32_t3 toEye = normalize(gCamera.position - input.worldPosition);

    float32_t4 resultColor;
    
    // 現状の通常
    resultColor = BlinnPhongReflection(input, textureColor, toEye);
    
    float32_t3 cameraToPosition = normalize(input.worldPosition - gCamera.position);
    float32_t3 relectedVector = reflect(cameraToPosition, normalize(input.normal));
    float32_t4 environmentColor = gEnvironmentTexture.Sample(gSampler, relectedVector);

    resultColor.rgb += environmentColor.rgb * gMaterial.coefficient;
    
    output.color = resultColor;
    output.color.a = gMaterial.color.a;
    
    if (output.color.a < 0.05f)
    {
        discard;
    }
    
    return output;

}