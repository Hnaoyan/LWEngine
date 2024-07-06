#pragma once
#include <vector>
#include <memory>
#include "../Drawer/Model.h"
#include "../../Base/Utility/DxCreateLib.h"

class InstancedUnit;

class InstancedGroup
{
public:
	// 最大数
	static const uint32_t kNumInstanceMax = 256;
	// パイプライン
	static GeneralPipeline sPipeline_;

private:
	// リソース
	Microsoft::WRL::ComPtr<ID3D12Resource> groupResources;
	CBufferInstancedUnit* unitDataMap_ = nullptr;

	// SRV情報
	std::pair<D3D12_CPU_DESCRIPTOR_HANDLE, D3D12_GPU_DESCRIPTOR_HANDLE> srvHandles_;
	uint32_t srvIndex;

	// Unitの数
	uint32_t unitNum_ = 0u;

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