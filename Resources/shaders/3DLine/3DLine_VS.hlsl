#include "3DLine.hlsli"

struct VSInput
{
    float32_t4 position : POSITION0;
    float32_t4 color : COLOR0;
};

// カメラ
ConstantBuffer<Camera> gCamera : register(b0);

VSOutput main(VSInput input)
{
    VSOutput output;
    float32_t4x4 viewProjection = mul(gCamera.view, gCamera.projection);
    output.position = mul(input.position, viewProjection);
    output.color = input.color;
    
    return output;
}