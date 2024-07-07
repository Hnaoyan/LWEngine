#pragma once
#include "Engine/3D/Instancing/InstancedUnit.h"

class Ground : public InstancedUnit
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Intialize() override;
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;



};
