#include "../3DModel.hlsli"

struct VSInput
{
    float32_t4 position : POSITION0;
    float32_t3 normal : NORMAL0;
    float32_t2 texcoord : TEXCOORD0;
    float32_t4 weight : WEIGHT0;
    int32_t4 index : INDEX0;
};

struct Skinned
{
    float32_t4 position;
    float32_t3 normal;
};

ConstantBuffer<Camera> gCamera : register(b0);
ConstantBuffer<WorldTransform> gWorldTransform : register(b1);
StructuredBuffer<Well> gMatrixPalette : register(t0);

Skinned Skinning(VSInput input)
{
    Skinned skinned;
    // 位置の変換
    skinned.position = mul(input.position, gMatrixPalette[input.index.x].skeletonSpaceMatrix) * input.weight.x;
    skinned.position += mul(input.position, gMatrixPalette[input.index.y].skeletonSpaceMatrix) * input.weight.y;
    skinned.position += mul(input.position, gMatrixPalette[input.index.z].skeletonSpaceMatrix) * input.weight.z;
    skinned.position += mul(input.position, gMatrixPalette[input.index.w].skeletonSpaceMatrix) * input.weight.w;
    skinned.position.w = 1.0f;
    
    // 法線の変換
    skinned.normal = mul(input.normal, (float32_t3x3) gMatrixPalette[input.index.x].skeletonSpaceInverseTransposeMatrix) * input.weight.x;
    skinned.normal += mul(input.normal, (float32_t3x3) gMatrixPalette[input.index.y].skeletonSpaceInverseTransposeMatrix) * input.weight.y;
    skinned.normal += mul(input.normal, (float32_t3x3) gMatrixPalette[input.index.z].skeletonSpaceInverseTransposeMatrix) * input.weight.z;
    skinned.normal += mul(input.normal, (float32_t3x3) gMatrixPalette[input.index.w].skeletonSpaceInverseTransposeMatrix) * input.weight.w;
    skinned.normal = normalize(skinned.normal); // 正規化して戻す
    
    return skinned;
};


VSOutput main(VSInput input)
{
    VSOutput output;
    Skinned skinned = Skinning(input);
    // ワールド座標計算
    float32_t4 worldPos = mul(skinned.position, gWorldTransform.worldMat);
    // 法線
    float32_t4x4 viewProjection = mul(gCamera.view, gCamera.projection);
    
    output.position = mul(worldPos, viewProjection);
    output.worldPosition = worldPos.xyz;
    output.texcoord = input.texcoord;
    output.normal = normalize(mul(skinned.normal, (float32_t3x3) gWorldTransform.WorldInverseTranspose));
  
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