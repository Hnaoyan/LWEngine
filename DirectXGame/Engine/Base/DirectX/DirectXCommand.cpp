#include "DirectXCommand.h"
#include <cassert>

ID3D12Device* DirectXCommand::device_ = nullptr;

Microsoft::WRL::ComPtr<ID3D12CommandQueue> DirectXCommand::commandQueue_;
Microsoft::WRL::ComPtr<ID3D12CommandAllocator> DirectXCommand::commandAllocator_;
Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> DirectXCommand::commandList_;

void DirectXCommand::Initialize(ID3D12Device* device)
{

	device_ = device;

	HRESULT result = S_FALSE;
	// コマンドキューを生成する
	D3D12_COMMAND_QUEUE_DESC commandQueueDesc{};
	result = device_->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(&commandQueue_));
	// コマンドキューの生成がうまくいかなかったので起動できない
	assert(SUCCEEDED(result));

	// コマンドアロケータを生成する
	result = device_->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocator_));
	// コマンドアロケータの生成がうまくいかなかったので起動できない
	assert(SUCCEEDED(result));

	// コマンドリストを生成する
	result = device_->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator_.Get(), nullptr, IID_PPV_ARGS(&commandList_));
	// コマンドリストの生成がうまくいかなかったので起動できない
	assert(SUCCEEDED(result));
}
