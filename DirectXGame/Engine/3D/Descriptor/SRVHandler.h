#pragma once
#include "../../Utility/Singleton.h"
#include "d3d12.h"
#include <wrl.h>
#include <stdint.h>
#include <array>

class DirectXDevice;

class SRVHandler : public Singleton<SRVHandler>
{
public:
	// 最大カウント
	static const int kDescpritorSize = 1024 * 10;
	//// 現在の番号
	//static uint32_t sNowDescriptorNum_;
	//// 次の番号
	//static uint32_t sNextDescriptorNum_;

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
		for (int i = 0; i < kDescpritorSize; ++i) {
			if (!sCheckLists_[i]) {
				sCheckLists_[i] = true;
				return i;
			}
		}
		return 0;
	}
	/// <summary>
	/// リリース時のヒープリストの処理
	/// </summary>
	/// <param name="index"></param>
	static void ReleaseHeapIndex(uint32_t index) {
		sCheckLists_[index] = false;
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
	// チェック用のリスト
	static std::array<bool, kDescpritorSize> sCheckLists_;
};

