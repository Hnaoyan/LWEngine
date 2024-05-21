#pragma once
#include "../3D/CBuffer.h"
#include <wrl.h>
#include <d3d12.h>
#include <numbers>

class ICamera
{
public:
	virtual void Initialize();
	virtual void Update();
protected:
	void Map();
	void CreateCBuffer();
	void UpdateMatrix();

public:
	// 
	Transform transform_ = {};

};
