#include "DirectXCommon.h"
#include "Utility/DxCreateLib.h"

#include <cassert>
#include <thread>

void DirectXCommon::Initialize(WindowAPI* winApp, int32_t bufferWidth, int32_t bufferHeight)
{
	assert(winApp);
	// 初期設定
	winApp_ = winApp;
	backBufferWidth_ = bufferWidth;
	backBufferHeight_ = bufferHeight;

	// フレームレートの設定
	InitializeFixFPS();
	// デバイス関係
	dxDevice_ = std::make_unique<DirectXDevice>();
	dxDevice_->Initialize();
	// コマンド関係
	dxCommand_ = std::make_unique<DirectXCommand>();
	dxCommand_->Initialize(dxDevice_->GetDevice());

	// 多用するもののポインタ
	device_ = dxDevice_->GetDevice();
	//commandList_ = dxCommand_->commandList_.Get();

	// SwapChain:Fence
	swapChainManager_ = std::make_unique<SwapChainManager>();
	swapChainManager_->Initialize(dxDevice_.get(), backBufferWidth_, backBufferHeight_);

#pragma region SRV:RTV:DSV生成
	srvHandler_ = std::make_unique<SRVHandler>();
	rtvHandler_ = std::make_unique<RTVHandler>();
	dsvHandler_ = std::make_unique<DSVHandler>();

	srvHandler_->StaticInitialize(dxDevice_.get());
	rtvHandler_->StaticInitialize(dxDevice_.get(), swapChainManager_.get());
	dsvHandler_->StaticInitialize(dxDevice_.get());
#pragma endregion

}

void DirectXCommon::PreDraw()
{
	
	UINT backBufferIndex = swapChainManager_->GetSwapChain()->GetCurrentBackBufferIndex();

	// Barrier
	D3D12_RESOURCE_BARRIER barrier = DxCreateLib::ResourceLib::GetResourceBarrier(rtvHandler_->GetBackBuffer(backBufferIndex),
		D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
	// TransionBarrier
	dxCommand_->sCommandList_->ResourceBarrier(1, &barrier);

	// ハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = DxCreateLib::DescriptorLib::GetCPUDescriptorHandle(rtvHandler_->GetRtvHeap(), device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV), backBufferIndex);
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = dsvHandler_->GetHeap()->GetCPUDescriptorHandleForHeapStart();
	// レンダーターゲットを設定
	dxCommand_->sCommandList_->OMSetRenderTargets(1, &rtvHandle, false, &dsvHandle);

	rtvHandler_->ClearRenderTarget(DirectXCommand::sCommandList_.Get());
	dsvHandler_->ClearDepthBuffer(DirectXCommand::sCommandList_.Get());
	//ClearDepthBuffer();

	// ビューポートの設定
	D3D12_VIEWPORT viewport = DxCreateLib::ViewLib::CreateViewport(FLOAT(backBufferWidth_), FLOAT(backBufferHeight_), 0, 0, 0.0f, 1.0f);
	D3D12_RECT scissorRect = DxCreateLib::ViewLib::CreateScissorRect(0, FLOAT(backBufferWidth_), 0, FLOAT(backBufferHeight_));
	dxCommand_->sCommandList_->RSSetViewports(1, &viewport);
	dxCommand_->sCommandList_->RSSetScissorRects(1, &scissorRect);


}

void DirectXCommon::PostDraw()
{
	HRESULT result = S_FALSE;

	UINT backBufferIndex = swapChainManager_->GetSwapChain()->GetCurrentBackBufferIndex();
	// Barrier
	D3D12_RESOURCE_BARRIER barrier = DxCreateLib::ResourceLib::GetResourceBarrier(rtvHandler_->GetBackBuffer(backBufferIndex),
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
	// TransionBarrier
	dxCommand_->sCommandList_->ResourceBarrier(1, &barrier);

	// コマンドリストの内容を確定させる。
	result = dxCommand_->sCommandList_->Close();
	assert(SUCCEEDED(result));

	ID3D12CommandList* commandLists[] = { DirectXCommand::sCommandList_.Get() };
	dxCommand_->sCommandQueue_->ExecuteCommandLists(1, commandLists);

	// GPUとOSに画面の交換を行うように
	swapChainManager_->GetSwapChain()->Present(1, 0);

	// FenceのSignal待ち
	HANDLE fenceEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	assert(fenceEvent != nullptr);

	// GPUがここまでたどり着いたときに
	dxCommand_->sCommandQueue_->Signal(swapChainManager_->GetFence(), ++fenceVal_);

	if (swapChainManager_->GetFence()->GetCompletedValue() != fenceVal_) 
	{
		// 指定した
		swapChainManager_->GetFence()->SetEventOnCompletion(fenceVal_, fenceEvent);
		// イベント待ち
		WaitForSingleObject(fenceEvent, INFINITE);
		CloseHandle(fenceEvent);
	}

	// FPS固定処理
	UpdateFixFPS();
	// 次フレーム用のコマンドリストを準備
	result = dxCommand_->sCommandAllocator_->Reset();
	assert(SUCCEEDED(result));
	result = dxCommand_->sCommandList_->Reset(DirectXCommand::sCommandAllocator_.Get(), nullptr);

}

void DirectXCommon::InitializeFixFPS()
{
	// 現在時間を記録する
	reference_ = std::chrono::steady_clock::now();
}

void DirectXCommon::UpdateFixFPS()
{
	// 1/60秒ぴったりの時間
	const std::chrono::microseconds kMinTime(uint64_t(1000000.0f / 60.0f));
	// 1/60秒よりわずかに短い時間
	const std::chrono::microseconds kMinCheckTime(uint64_t(1000000.0f / 75.0f));

	// 現在時間を取得
	std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
	// 前回記録からの経過時間を取得する
	std::chrono::microseconds elapsed =
		std::chrono::duration_cast<std::chrono::microseconds>(now - reference_);

	// 1/60秒（よりわずかに短い時間）経っていない場合
	if (elapsed < kMinCheckTime) {
		// 1/60秒経過するまで微小なスリープを繰り返す
		while (std::chrono::steady_clock::now() - reference_ < kMinCheckTime) {
			// 1マイクロ秒スリープ
			std::this_thread::sleep_for(std::chrono::microseconds(1));
		}
	}
	// 現在の時間を記録する
	reference_ = std::chrono::steady_clock::now();
}
