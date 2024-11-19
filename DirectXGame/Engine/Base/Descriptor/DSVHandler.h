#pragma once
#include "../../Utility/Singleton.h"
#include <d3d12.h>
#include <wrl.h>
#include <cstdint>

class DirectXDevice;

/// <summary>
/// DSV管理
/// </summary>
class DSVHandler : public Singleton<DSVHandler>
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="dxDevice"></param>
	void StaticInitialize(DirectXDevice* dxDevice);
	/// <summary>
	/// デプスバッファーのクリア
	/// </summary>
	/// <param name="cmdList"></param>
	void ClearDepthBuffer(ID3D12GraphicsCommandList* cmdList);
	// ヒープの取得
	ID3D12DescriptorHeap* GetHeap() { return dsvHeap_.Get(); }

private:
	/// <summary>
	/// バッファーの作成
	/// </summary>
	void CreateDepthBuffer();

private:
	// デバイス管理クラス
	DirectXDevice* dxDevice_ = nullptr;
	// ヒープ自体
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> dsvHeap_;

	// インフォ
	uint32_t kDescriptorSizeDSV_ = 0;
	uint32_t size_ = 0;
	uint32_t bufferWidth_ = 0;
	uint32_t bufferHeight_ = 0;

	Microsoft::WRL::ComPtr<ID3D12Resource> depthStencilResource_;
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc_{};

};