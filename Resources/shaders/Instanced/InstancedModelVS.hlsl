#include "Instanced.hlsli"

struct VSInput
{
    float32_t4 position : POSITION0;
    float32_t3 normal : NORMAL0;
    float32_t2 texcoord : TEXCOORD0;
    float32_t4 color : COLOR0;
};

ConstantBuffer<Camera> gCamera : register(b0);
StructuredBuffer<WorldTransform> gWorldTransform : register(t0);

VSOutput main(VSInput input, uint32_t instanceId : SV_InstanceID)
{
    VSOutput output;
    // ワールド座標計算
    float32_t4 worldPos = mul(input.position, gWorldTransform[instanceId].worldMat);
    // 法線
    float32_t3 worldNormal = normalize(mul(input.normal, (float32_t3x3) gWorldTransform[instanceId].WorldInverseTranspose));

    float32_t4x4 viewProjection = mul(gCamera.view, gCamera.projection);
    float32_t4x4 worldViewProjection = mul(gWorldTransform[instanceId].worldMat, viewProjection);

    output.position = mul(input.position, worldViewProjection);
    output.normal = worldNormal;
    output.texcoord = input.texcoord;
    output.color = input.color;
    output.worldPosition = worldPos.xyz;
    
    return output;
}
