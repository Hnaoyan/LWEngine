#pragma once
#include "../Utility/Singleton.h"

#include<d3d12.h>
#include<dxgi1_6.h>

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

class DirectionalLight;

class LightingManager : public Singleton<LightingManager>
{
public:
	struct LightDesc
	{
		ID3D12GraphicsCommandList* commandList = nullptr; // コマンドリスト
		DirectionalLight* directionalLight = nullptr; // 平行光源

	};

public:
	//void StaticInitialize(ID3D12Device* device);

private:

	//// デバイス
	//static ID3D12Device* sDevice;
	//// コマンドリスト
	//static ID3D12GraphicsCommandList* sCommandList;

};