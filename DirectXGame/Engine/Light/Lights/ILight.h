#pragma once
#include <stdint.h>
#include <wrl.h>
#include <d3d12.h>
#include <dxgi1_6.h>

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

#include "../../Base/DirectXCommon.h"

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
	Microsoft::WRL::ComPtr<ID3D12Resource> lightBuff_;
	T* lightDataMap_ = nullptr;

};

template<typename T>
inline void ILight<T>::Initialize()
{
	device = DirectXCommon::GetInstance()->GetDevice();

	// 平行光源リソースを作る
	lightBuff_ = DxCreateLib::ResourceLib::CreateBufferResource(device, sizeof(T));

	// 書き込むためのアドレスを取得
	lightBuff_->Map(0, nullptr, reinterpret_cast<void**>(&lightDataMap_));

}

template<typename T>
inline void ILight<T>::Draw(ID3D12GraphicsCommandList* cmdList, uint32_t rootParamIndex)
{
	// チェック
	assert(sCommandList == nullptr);
	// コマンドリストの登録
	sCommandList = cmdList;
	// CBufferの設定
	sCommandList->SetGraphicsRootConstantBufferView(static_cast<UINT>(rootParamIndex), lightBuff_->GetGPUVirtualAddress());
	// コマンドリストの解除
	sCommandList = nullptr;
}
