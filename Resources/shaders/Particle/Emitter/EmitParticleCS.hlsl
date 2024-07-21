#include "../Particle.hlsli"

ConstantBuffer<EmitterSphere> gEmitter : register(b0);

[numthreads(1, 1, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
}