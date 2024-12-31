#pragma once
#include "Engine/3D/Instancing/InstancedUnit.h"

class CPUParticleUnit : public InstancedUnit
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model"></param>
	void Initialize() override;
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

};
