#include "Skybox.hlsli"

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
    float32_t3 worldNormal = normalize(mul(input.normal, (float32_t3x3) gWorldTransform.worldMat));

    float32_t4x4 viewProjection = mul(gCamera.view, gCamera.projection);
    float32_t4x4 worldViewProjection = mul(gWorldTransform.worldMat, viewProjection);
    
    output.position = mul(input.position, worldViewProjection).xyww;
    output.texcoord = input.texcoord;
    
    return output;
}
