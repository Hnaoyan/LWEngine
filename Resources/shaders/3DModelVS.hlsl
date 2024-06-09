#include "3DModel.hlsli"

struct VSInput
{
    float32_t4 position : POSITION0;
    float32_t3 normal : NORMAL0;
    float32_t2 texcoord : TEXCOORD0;
};

ConstantBuffer<WorldTransform> gWorldTransform : register(b0);
ConstantBuffer<Camera> gCamera : register(b1);

VSOutput main(VSInput input)
{
    VSOutput output;
    // ワールド座標計算
    float32_t4 worldPos = mul(input.position, gWorldTransform.worldMat);
    // 法線
    float32_t3 worldNormal = normalize(mul(input.normal, (float32_t3x3)gWorldTransform.worldMat));

    float32_t4x4 viewProjection = mul(gCamera.view, gCamera.projection);
    
    output.position = /*mul(input.position, */mul(worldPos, viewProjection);
    output.normal = worldNormal;
    output.texcoord = input.texcoord;
    output.worldPosition = worldPos.xyz;
    
    return output;
}

//VertexShaderOutput main(VertexShaderInput input)
//{
//    VertexShaderOutput output;
//    output.position = mul(input.position, gTransformationMatrix.WVP);
//    output.texcoord = input.texcoord;
//    output.normal = normalize(mul(input.normal, (float32_t3x3) gTransformationMatrix.World));
//    return output;
//}