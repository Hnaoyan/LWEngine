#pragma once
#include "../Utility/Singleton.h"
#include <d3d12.h>

class SRVHandler;

class PostEffectRender : public Singleton<PostEffectRender>
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="device"></param>
	/// <param name="commandList"></param>
	void Initialize(ID3D12Device* device, ID3D12GraphicsCommandList* commandList);

	//void Draw(SRVHandler* srv);

private:
	ID3D12Device* device_ = nullptr;
	ID3D12GraphicsCommandList* commandList_ = nullptr;


};