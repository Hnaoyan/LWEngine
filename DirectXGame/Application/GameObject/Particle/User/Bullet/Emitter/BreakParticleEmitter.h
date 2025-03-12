#pragma once
#include "../../../CPU/Emitter/CPUParticleEmitter.h"

/// <summary>
/// 壊れるときのエフェクト
/// </summary>
class BreakParticleEmitter : public CPUParticleEmitter
{
public:
	void Initialize(Model* model) override;
	void Update() override;
	void Draw(ModelDrawDesc desc) override;
	void EmitParticles() override;
private:

};