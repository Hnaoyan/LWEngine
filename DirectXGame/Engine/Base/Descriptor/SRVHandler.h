#pragma once
#include "../../Utility/Singleton.h"
#include "Engine/Utility/IndexAllocator.h"
#include "d3d12.h"
#include <wrl.h>
#include <stdint.h>
#include <array>
#include <cassert>
#include <memory>

class DirectXDevice;

/// <summary>
/// SRV管理
/// </summary>
class SRVHandler : public Singleton<SRVHandler>
{
public:
	// 最大カウント
	static const int kDescpritorSize = 1024 * 10;
	// アロケータ
	static std::unique_ptr<IndexAllocator> sAllocator;

public:
	/// <summary>
	/// 静的初期化
	/// </summary>
	/// <param name="dxDevice"></param>
	static void StaticInitialize(DirectXDevice* dxDevice);

public: // アクセッサ

	/*ハンドルを取得後にヒープ番号を進めるようにすること*/

	/// <summary>
	/// ハンドルの取得
	/// </summary>
	/// <returns></returns>
	static D3D12_CPU_DESCRIPTOR_HANDLE GetSrvHandleCPU(uint32_t index);

	static D3D12_GPU_DESCRIPTOR_HANDLE GetSrvHandleGPU(uint32_t index);

	/// <summary>
	/// ヒープのリストから空いてる番号を探す
	/// </summary>
	/// <returns></returns>
	static uint32_t CheckAllocater() {
		//assert(sAllocator->AllocateIndex() >= 0);
		return (uint32_t)sAllocator->AllocateIndex();
	}
	/// <summary>
	/// リリース時のヒープリストの処理
	/// </summary>
	/// <param name="index"></param>
	static void ReleaseHeapIndex(uint32_t index) {
		//sCheckLists_[index] = false;
		sAllocator->ReleaseIndex((int32_t)index);
	}

	/// <summary>
	/// SRVヒープの取得
	/// </summary>
	/// <returns></returns>
	ID3D12DescriptorHeap* GetHeap() { return srvHeap_.Get(); }
private:
	// デバイスクラス
	static DirectXDevice* dxDevice_;
	// DescriptorHeap
	static Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvHeap_;
	// サイズ
	static uint32_t kDescriptorSizeSRV_;
};

