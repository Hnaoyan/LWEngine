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

public:	// メンバ変数
	// キュー
	static Microsoft::WRL::ComPtr<ID3D12CommandQueue> sCommandQueue_;
	// アロケータ
	static Microsoft::WRL::ComPtr<ID3D12CommandAllocator> sCommandAllocator_;
	// リスト
	static Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> sCommandList_;

private: // 

	static ID3D12Device* device_;

};