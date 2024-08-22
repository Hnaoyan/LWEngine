#pragma once
#include "../../Utility/Singleton.h"
#include "d3d12.h"
#include <wrl.h>
#include <stdint.h>

class DirectXDevice;

class SRVHandler : public Singleton<SRVHandler>
{
public:
	// 最大カウント
	static const int kDescpritorSize = 1024 * 2;
	// 現在の番号
	static uint32_t sNowDescriptorNum_;
	// 次の番号
	static uint32_t sNextDescriptorNum_;

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
	static D3D12_CPU_DESCRIPTOR_HANDLE GetSrvHandleCPU();
	static D3D12_GPU_DESCRIPTOR_HANDLE GetSrvHandleGPU();

	/// <summary>
	/// ヒープ番号進めて値を取得
	/// </summary>
	/// <returns></returns>
	static uint32_t AllocateDescriptor() {
		// 現在の値
		sNowDescriptorNum_ = sNextDescriptorNum_;
		// 進める
		sNextDescriptorNum_++;

		return sNowDescriptorNum_;
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

