#include "../User/PlayerEffect.hlsl"

static const uint32_t kMaxParticle = 1024;

ConstantBuffer<EmitterSphere> gEmitter : register(b0);
ConstantBuffer<PerFrame> gPerFrame : register(b1);
RWStructuredBuffer<Particle> gParticle : register(u0);
RWStructuredBuffer<int32_t> gFreeListIndex : register(u1);
RWStructuredBuffer<uint32_t> gFreeList : register(u2);

[numthreads(1, 1, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
    if (gEmitter.emit != 0)
    {
        RandomGenerator generator;
        generator.seed = (DTid + gPerFrame.time) * gPerFrame.time;
        
        for (uint32_t countIndex = 0; countIndex < gEmitter.count; ++countIndex)
        {
            int32_t freeListIndex;
            // リストの一個前を設定してIndexを取得
            InterlockedAdd(gFreeListIndex[0], -1, freeListIndex);
            // 空いている場合
            if (0 <= freeListIndex && freeListIndex < kMaxParticle)
            {
                uint32_t particleIndex = gFreeList[freeListIndex];
                // 初期化処理
                float32_t value = 0.0f;
                // デフォルト
                if (gEmitter.emitPattern == 0)
                {
                    gParticle[particleIndex] = DefaultPop(generator);
                }
                // プレイヤーの移動
                else if(gEmitter.emitPattern == 1)
                {
                    gParticle[particleIndex] = PlayerMove(generator, gEmitter.translate);
                }
                // プレイヤーの弾
                else if(gEmitter.emitPattern == 2)
                {
                    gParticle[particleIndex] = PlayerBullet(generator, gEmitter.translate);
                }
                // ボスのヒットエフェクト
                else if (gEmitter.emitPattern == 3)
                {
                    gParticle[particleIndex] = BossDamage(generator, gEmitter.translate);
                }
                // 弾が壊れるエフェクト
                else if (gEmitter.emitPattern == 4)
                {
                    gParticle[particleIndex] = BulletBreak(generator, gEmitter.translate);
                }
                // ボスの弾
                else if (gEmitter.emitPattern == 5)
                {
                    gParticle[particleIndex] = BossBullet(generator, gEmitter.translate);
                }
                else if (gEmitter.emitPattern == 6)
                {
                    gParticle[particleIndex] = BossBarrierBreak(generator, gEmitter.translate);
                }
                float32_t3 test = generator.Generate3D();
            }
            // 空いていない場合
            else
            {
                // 発生させられなかったので、減らしてしまった分もとに戻す。
                InterlockedAdd(gFreeListIndex[0], 1);
                // Emit中にParticleは消えないので、この後発生することはないため終了
                break;
            }
        }
    }  
}
