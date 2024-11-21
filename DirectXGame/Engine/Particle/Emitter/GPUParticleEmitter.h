#pragma once
#include "Engine/Base/CBufferCommon.h"
#include "Engine/3D/Drawer/Model.h"

class GPUParticleManager;

class GPUParticleEmitter
{
public:
	// コマンドリスト
	static ID3D12GraphicsCommandList* sCommandList;
	// パイプライン
	static BlendPipeline sPipeline_;

	virtual ~GPUParticleEmitter() {
		// パーティクル関係
		SRVHandler::ReleaseHeapIndex(particles_.srvHeapData.index);
		SRVHandler::ReleaseHeapIndex(particles_.uavHeapData.index);
		// インデックス関係
		SRVHandler::ReleaseHeapIndex(freeListIndex_.srvHeapData.index);
		SRVHandler::ReleaseHeapIndex(freeListIndex_.uavHeapData.index);
		SRVHandler::ReleaseHeapIndex(freeList_.srvHeapData.index);
		SRVHandler::ReleaseHeapIndex(freeList_.uavHeapData.index);
	}

	using BlendMode = Pipeline::BlendMode;
private: // リソース
	// データ作成
	void CreateData();
	// GPUの初期化
	void GPUInitialize();
	// UAV用のバリア張り
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
	// エミッターのデータ
	EmitterSphere data_{};

	// 
	GPUParticleManager* gpuParticleManager_ = nullptr;

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
	/// <summary>
	/// エミッターの更新
	/// </summary>
	void UpdataEmitterFlags();
	
	bool IsDead() const { return isDead_; }
	void SetIsDead(bool isDead) { isDead_ = isDead; }
	void SetGPUParticleSystem(GPUParticleManager* system) { gpuParticleManager_ = system; }

protected: // USER
	// パーティクルで使うオブジェクト
	Model* model_ = nullptr;
	// テクスチャ
	uint32_t texture_ = 0u;
	// ブレンドモード
	BlendMode blendMode_ = BlendMode::kAlpha;
	// ビルボードの切り替え
	bool isBillBoard_ = false;
	// 消すフラグ
	bool isDead_ = false;

public: // アクセッサ
	// テクスチャを外部設定
	void SetTextureHandle(uint32_t texture) { texture_ = texture; }
};