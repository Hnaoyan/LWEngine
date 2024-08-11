#pragma once
#include <stdint.h>
#include <wrl.h>
#include <d3d12.h>
#include <dxgi1_6.h>

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

#include "../../Base/DirectXCommon.h"
#include "Engine/Base/CBufferCommon.h"

template<typename T>
class ILight
{
public:
	// コマンドリスト
	inline static ID3D12GraphicsCommandList* sCommandList;
	// デバイス
	inline static ID3D12Device* device = nullptr;
	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize();
	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="data"></param>
	virtual void Update(const T& data) = 0;
	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="cmdList"></param>
	/// <param name="rootParamIndex"></param>
	virtual void Draw(ID3D12GraphicsCommandList* cmdList, uint32_t rootParamIndex);

public:
	ConstantBufferMapContext<T> data_;
};

template<typename T>
inline void ILight<T>::Initialize()
{
	// デバイス
	device = DirectXCommon::GetInstance()->GetDevice();
	// バッファ作成
	data_.CreateConstantBuffer(device);
}

template<typename T>
inline void ILight<T>::Draw(ID3D12GraphicsCommandList* cmdList, uint32_t rootParamIndex)
{
	// チェック
	assert(sCommandList == nullptr);
	// コマンドリストの登録
	sCommandList = cmdList;
	// CBufferの設定
	sCommandList->SetGraphicsRootConstantBufferView(static_cast<UINT>(rootParamIndex), data_.cBuffer->GetGPUVirtualAddress());
	// コマンドリストの解除
	sCommandList = nullptr;
}
