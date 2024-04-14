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
	static Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue_;
	// アロケータ
	static Microsoft::WRL::ComPtr<ID3D12CommandAllocator> commandAllocator_;
	// リスト
	static Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList_;

private: // 

	static ID3D12Device* device_;

};