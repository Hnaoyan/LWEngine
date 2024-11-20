#pragma once
#include <wrl.h>
#include <dxgi1_6.h>
#include <d3d12.h>

#include "../../Utility/Singleton.h"

/// <summary>
/// DirectXデバイス
/// </summary>
class DirectXDevice : public Singleton<DirectXDevice>
{
public: // 関数
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

public: // アクセッサ
	/// <summary>
	/// ファクトリ
	/// </summary>
	/// <returns></returns>
	IDXGIFactory7* GetDXGIFactory() { return dxgiFactory_.Get(); }
	/// <summary>
	/// デバイス
	/// </summary>
	/// <returns></returns>
	ID3D12Device* GetDevice() { return device_.Get(); }

private: // 変数
	Microsoft::WRL::ComPtr<IDXGIFactory7> dxgiFactory_;
	Microsoft::WRL::ComPtr<ID3D12Device> device_;

};

