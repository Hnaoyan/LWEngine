#include "3DTriangle.hlsli"

struct VSInput
{
    float32_t3 position : POSITION0;
    float32_t4 color : COLOR0;
    float32_t2 texcoord : TEXCOORD0;
};

// カメラ
ConstantBuffer<Camera> gCamera : register(b0);

VSOutput main(VSInput input)
{
    VSOutput output;
    float32_t4x4 viewProjection = mul(gCamera.view, gCamera.projection);
    output.position = mul(float32_t4(input.position, 1.0f), viewProjection);
    output.color = input.color;
    output.texcoord = input.texcoord;
    
    return output;
}