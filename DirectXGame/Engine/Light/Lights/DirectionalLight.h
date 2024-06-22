#pragma once
#include "../LightData.h"

#include <wrl.h>
#include<d3d12.h>
#include<dxgi1_6.h>

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

class DirectionalLight
{
public:
	/// <summary>
	/// 作成関数
	/// </summary>
	/// <returns></returns>
	static DirectionalLight* CreateLight();
	// コマンドリスト
	static ID3D12GraphicsCommandList* sCommandList;
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="data"></param>
	void Update(const CBufferDataDirectionalLight& data);
	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name="cmdList"></param>
	/// <param name="rootParamIndex"></param>
	void Draw(ID3D12GraphicsCommandList* cmdList,uint32_t rootParamIndex);

public:
	Microsoft::WRL::ComPtr<ID3D12Resource> directionalLightBuff_;
	CBufferDataDirectionalLight* directionalLightMap_ = nullptr;

};

