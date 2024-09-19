#include "DirectXCommand.h"
#include "../DirectXCommon.h"
#include <cassert>

ID3D12Device* DirectXCommand::device_ = nullptr;

Microsoft::WRL::ComPtr<ID3D12CommandQueue> DirectXCommand::sCommandQueue_;
Microsoft::WRL::ComPtr<ID3D12CommandAllocator> DirectXCommand::sCommandAllocator_;
Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> DirectXCommand::sCommandList_;
Microsoft::WRL::ComPtr<ID3D12CommandAllocator> DirectXCommand::sCommandLoadAllocator_;
Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> DirectXCommand::sCommandLoadList_;

void DirectXCommand::Initialize(ID3D12Device* device)
{

	device_ = device;

	HRESULT result = S_FALSE;
	// コマンドキューを生成する
	D3D12_COMMAND_QUEUE_DESC commandQueueDesc{};
	result = device_->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(&sCommandQueue_));
	// コマンドキューの生成がうまくいかなかったので起動できない
	assert(SUCCEEDED(result));

	// コマンドアロケータを生成する
	result = device_->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&sCommandAllocator_));
	// コマンドアロケータの生成がうまくいかなかったので起動できない
	assert(SUCCEEDED(result));
	// コマンドリストを生成する
	result = device_->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, sCommandAllocator_.Get(), nullptr, IID_PPV_ARGS(&sCommandList_));
	// コマンドリストの生成がうまくいかなかったので起動できない
	assert(SUCCEEDED(result));

	// コマンドアロケータを生成する
	result = device_->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&sCommandLoadAllocator_));
	// コマンドアロケータの生成がうまくいかなかったので起動できない
	assert(SUCCEEDED(result));
	// コマンドリストを生成する
	result = device_->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, sCommandLoadAllocator_.Get(), nullptr, IID_PPV_ARGS(&sCommandLoadList_));
	// コマンドリストの生成がうまくいかなかったので起動できない
	assert(SUCCEEDED(result));
}

void DirectXCommand::ExecuteCommandList(ID3D12GraphicsCommandList* commandList)
{

	HRESULT result = S_FALSE;
	// コマンドリストの内容を確定させる。
	result = commandList->Close();
	assert(SUCCEEDED(result));
	// コマンドキューに内容を送る
	ID3D12CommandList* commandLists[] = { commandList };
	sCommandQueue_->ExecuteCommandLists(1, commandLists);
}

void DirectXCommand::WaitForFenceComplete()
{
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	// FenceのSignal待ち
	HANDLE fenceEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	assert(fenceEvent != nullptr);

	// GPUがここまでたどり着いたときに
	UINT value = dxCommon->GetFenceVal();
	sCommandQueue_->Signal(dxCommon->GetSwapChainManager()->GetFence(), ++value);
	// チェック
	if (dxCommon->GetSwapChainManager()->GetFence()->GetCompletedValue() != value)
	{
		// 指定した
		dxCommon->GetSwapChainManager()->GetFence()->SetEventOnCompletion(value, fenceEvent);
		// イベント待ち
		WaitForSingleObject(fenceEvent, INFINITE);
		CloseHandle(fenceEvent);
	}
	// 新しい値を設定
	dxCommon->SetFenceVal(value);
}

void DirectXCommand::ResetCloseCommandList(ID3D12GraphicsCommandList* commandList)
{
	// 描画用にコマンドリストリセット
	HRESULT result = S_FALSE;
	result = sCommandAllocator_->Reset();
	assert(SUCCEEDED(result));
	result = commandList->Reset(sCommandAllocator_.Get(), nullptr);
}
