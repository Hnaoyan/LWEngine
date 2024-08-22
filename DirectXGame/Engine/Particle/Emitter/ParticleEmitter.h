#pragma once
#include "Engine/Base/CBufferCommon.h"
#include "Engine/3D/Drawer/Model.h"

class ParticleEmitter
{
public:
	// コマンドリスト
	static ID3D12GraphicsCommandList* sCommandList;
	// パイプライン
	static GeneralPipeline sPipeline_;

	virtual ~ParticleEmitter() {
		// パーティクル関係
		SRVHandler::ReleaseHeapIndex(particles_.srvHeapData.index);
		SRVHandler::ReleaseHeapIndex(particles_.uavHeapData.index);
		// インデックス関係
		SRVHandler::ReleaseHeapIndex(freeListIndex_.srvHeapData.index);
		SRVHandler::ReleaseHeapIndex(freeListIndex_.uavHeapData.index);
		SRVHandler::ReleaseHeapIndex(freeList_.srvHeapData.index);
		SRVHandler::ReleaseHeapIndex(freeList_.uavHeapData.index);
	}

public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model"></param>
	/// <param name="textureHandle"></param>
	virtual void Initialize(Model* model,uint32_t textureHandle = 0);
	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update();
	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera"></param>
	virtual void Draw(ICamera* camera);
public:
	/// <summary>
	/// データの更新
	/// </summary>
	/// <param name="data"></param>
	void RefreshData(const EmitterSphere& data);

	void UpdataEmitterFlags();
private:
	/// <summary>
	/// データ作成
	/// </summary>
	void CreateData();
	void GPUInitialize();
	void BarrierUAV();

	// パーティクルのリソース
	RWStructuredBufferContext<ParticleCS> particles_;
	RWStructuredBufferContext<int32_t> freeListIndex_;
	RWStructuredBufferContext<uint32_t> freeList_;

protected:
	// エミッターの種類（今後の変更する可能性あり
	ConstantBufferMapContext<EmitterSphere> emitter_;
	// 共通のバッファー
	ConstantBufferMapContext<PerView> perView_;
	ConstantBufferMapContext<PerFrame> perFrame_;

protected:
	// パーティクルで使うオブジェクト
	Model* model_ = nullptr;
	// テクスチャ
	uint32_t texture_ = 0u;
};