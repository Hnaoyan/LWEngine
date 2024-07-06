#pragma once
#include <vector>
#include <memory>
#include "../Drawer/Model.h"
#include "InstancingData.h"
#include "../../Base/Utility/DxCreateLib.h"

class InstancedUnit;

class InstancedGroup
{
public:
	// 最大数
	static const uint32_t kNumInstanceMax = 256;

private:
	Microsoft::WRL::ComPtr<ID3D12Resource> groupResources;
	CBufferInstancedUnit* unitDataMap_ = nullptr;

	std::pair<D3D12_CPU_DESCRIPTOR_HANDLE, D3D12_GPU_DESCRIPTOR_HANDLE> srvHandles_;
	uint32_t srvIndex;

public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model"></param>
	virtual void Initialize(Model* model);
	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update();
	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="desc"></param>
	virtual void Draw(ModelDrawDesc desc);

private:
	/// <summary>
	/// インスタンシングのSRV確保など
	/// </summary>
	void CreateData();

private:
	// ユニットリスト
	std::vector<std::unique_ptr<InstancedUnit>> units_;
	// モデル
	Model* model_ = nullptr;

};