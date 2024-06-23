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
	static const int kDescpritorSize = 512;
	// 次の番号
	static uint32_t sNextDescriptorNum_;

public:
	/// <summary>
	/// ヒープ番号進めて値を取得
	/// </summary>
	/// <returns></returns>
	static uint32_t AllocateDescriptor() {
		// 現在の値
		uint32_t result = sNextDescriptorNum_;
		// 進める
		sNextDescriptorNum_++;

		return result;
	}

	/// <summary>
	/// 静的初期化
	/// </summary>
	/// <param name="dxDevice"></param>
	static void StaticInitialize(DirectXDevice* dxDevice);

public: // アクセッサ
	/// <summary>
	/// ハンドルの取得
	/// </summary>
	/// <returns></returns>
	static D3D12_CPU_DESCRIPTOR_HANDLE GetSrvHandleCPU();
	static D3D12_GPU_DESCRIPTOR_HANDLE GetSrvHandleGPU();
	/// <summary>
	/// SRVヒープの取得
	/// </summary>
	/// <returns></returns>
	ID3D12DescriptorHeap* GetSrvHeap() { return srvHeap_.Get(); }
private:
	// デバイスクラス
	static DirectXDevice* dxDevice_;
	// DescriptorHeap
	static Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvHeap_;

	//uint32_t index_ = 0;
	static uint32_t kDescriptorSizeSRV_;

};

