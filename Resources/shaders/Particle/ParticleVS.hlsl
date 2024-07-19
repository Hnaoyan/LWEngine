#include "Particle.hlsli"

struct ParticleForGPU
{
    float32_t4x4 World;
    float32_t4 Color;
};

StructuredBuffer<Particle> gParticles : register(t0);
ConstantBuffer<PerView> gPerView : register(b0);

struct VertexShaderInput
{
    float32_t4 position : POSITION0;
    float32_t3 normal : NORMAL0;
    float32_t2 texcoord : TEXCOORD0;
};

//ConstantBuffer<

VertexShaderOutput main(VertexShaderInput input, uint32_t instanceId : SV_InstanceID)
{
    VertexShaderOutput output;
    Particle particle = gParticles[instanceId];
    // 行列作成
    float32_t4x4 worldMatrix = gPerView.billboardMatrix;
    worldMatrix[0] *= particle.scale.x;
    worldMatrix[1] *= particle.scale.y;
    worldMatrix[2] *= particle.scale.z;
    worldMatrix[3].xyz *= particle.translate;
    output.position = mul(input.position, mul(worldMatrix, gPerView.viewProjection));
    output.normal = normalize(mul(input.normal, (float32_t3x3) worldMatrix));
    output.texcoord = input.texcoord;
    output.color = particle.color;
    //output.position = mul(input.position, gParticle[instanceId].World);
    //output.texcoord = input.texcoord;
    //output.normal = normalize(mul(input.normal, (float32_t3x3) gParticle[instanceId].World));
    //output.color = gParticle[instanceId].Color;
    return output;
}