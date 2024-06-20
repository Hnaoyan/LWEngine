#pragma once
#include "../LightData.h"

#include <wrl.h>
#include<d3d12.h>
#include<dxgi1_6.h>

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

class DirectionLight
{
public:

	//static DirectionLight* Create();

public:
	//void Initialize();
	//void Update();
	//void Draw();

public:
	Microsoft::WRL::ComPtr<ID3D12Resource> directionLightBuff_;
	CBufferDataDirectionalLight* directionLightMap_ = nullptr;

};

