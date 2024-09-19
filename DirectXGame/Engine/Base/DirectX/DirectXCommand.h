#pragma once
#include "../../Utility/Singleton.h"

#include <wrl.h>
#include <d3d12.h>

class DirectXCommand : public Singleton<DirectXCommand>
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="device"></param>
	static void Initialize(ID3D12Device* device);
	/// <summary>
	/// コマンドリストの確定と送信
	/// </summary>
	/// <param name="commandList"></param>
	static void ExecuteCommandList(ID3D12GraphicsCommandList* commandList);
	/// <summary>
	/// コマンドリストの完了を確認
	/// </summary>
	static void WaitForFenceComplete();
	/// <summary>
	/// コマンドリストの準備
	/// </summary>
	/// <param name="commandList"></param>
	static void ResetCloseCommandList(ID3D12GraphicsCommandList* commandList);
	static void ResetCloseCommandList(ID3D12GraphicsCommandList* commandList, ID3D12CommandAllocator* allocator);

public:	// メンバ変数
	// キュー
	static Microsoft::WRL::ComPtr<ID3D12CommandQueue> sCommandQueue_;
	// アロケータ
	static Microsoft::WRL::ComPtr<ID3D12CommandAllocator> sCommandAllocator_;
	// リスト
	static Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> sCommandList_;
	// ロード用のリスト
	static Microsoft::WRL::ComPtr<ID3D12CommandAllocator> sCommandLoadAllocator_;
	static Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> sCommandLoadList_;
	// GPU
	static Microsoft::WRL::ComPtr<ID3D12CommandAllocator> sCommandGPUAllocator_;
	static Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> sCommandGPUList_;

private: // 

	static ID3D12Device* device_;

};