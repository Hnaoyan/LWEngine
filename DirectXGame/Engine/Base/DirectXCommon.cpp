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
	DirectXCommand::sCommandList_->ResourceBarrier(1, &barrier);

	// ハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = DxCreateLib::DescriptorLib::GetCPUDescriptorHandle(rtvHandler_->GetHeap(), device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV), backBufferIndex);
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = dsvHandler_->GetHeap()->GetCPUDescriptorHandleForHeapStart();
	// レンダーターゲットを設定
	DirectXCommand::sCommandList_->OMSetRenderTargets(1, &rtvHandle, false, &dsvHandle);

	rtvHandler_->ClearRenderTarget(DirectXCommand::sCommandList_.Get());
	dsvHandler_->ClearDepthBuffer(DirectXCommand::sCommandList_.Get());
	//ClearDepthBuffer();

	// ビューポートの設定
	D3D12_VIEWPORT viewport = DxCreateLib::ViewLib::CreateViewport(FLOAT(backBufferWidth_), FLOAT(backBufferHeight_), 0, 0, 0.0f, 1.0f);
	D3D12_RECT scissorRect = DxCreateLib::ViewLib::CreateScissorRect(0, FLOAT(backBufferWidth_), 0, FLOAT(backBufferHeight_));
	DirectXCommand::sCommandList_->RSSetViewports(1, &viewport);
	DirectXCommand::sCommandList_->RSSetScissorRects(1, &scissorRect);


}

void DirectXCommon::PostDraw()
{
	// バッファーのインデックス
	UINT backBufferIndex = swapChainManager_->GetSwapChain()->GetCurrentBackBufferIndex();
	// Barrier
	D3D12_RESOURCE_BARRIER barrier = DxCreateLib::ResourceLib::GetResourceBarrier(rtvHandler_->GetBackBuffer(backBufferIndex),
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
	// TransionBarrier
	dxCommand_->sCommandList_->ResourceBarrier(1, &barrier);
	// コマンドリストの確定
	DirectXCommand::ExecuteCommandList(DirectXCommand::sCommandList_.Get());
	// GPUとOSに画面の交換を行うように
	swapChainManager_->GetSwapChain()->Present(1, 0);
	// イベント待ち確認
	DirectXCommand::WaitForFenceComplete();
	// FPS固定処理
	UpdateFixFPS();
	// コマンドリストのリセット
	DirectXCommand::ResetCloseCommandList(DirectXCommand::sCommandList_.Get());
}

void DirectXCommon::RenderPreDraw()
{
	// フェンスで終了の確認
	DirectXCommand::WaitForFenceComplete();
	// コマンドリストのリセット
	DirectXCommand::ResetCloseCommandList(DirectXCommand::sCommandList_.Get());
	// Barrier
	D3D12_RESOURCE_BARRIER barrier = DxCreateLib::ResourceLib::GetResourceBarrier(rtvHandler_->GetRenderTexture(),
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET);
	// TransitionBarrierを張る
	DirectXCommand::sCommandList_->ResourceBarrier(1, &barrier);
	// 描画先のRTVを設定する
	// ハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = DxCreateLib::DescriptorLib::GetCPUDescriptorHandle(rtvHandler_->GetHeap(), device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV), 2);
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = dsvHandler_->GetHeap()->GetCPUDescriptorHandleForHeapStart();
	// レンダーターゲットを設定
	DirectXCommand::sCommandList_->OMSetRenderTargets(1, &rtvHandle, false, &dsvHandle);

	rtvHandler_->ClearRenderTexture(DirectXCommand::sCommandList_.Get());
	dsvHandler_->ClearDepthBuffer(DirectXCommand::sCommandList_.Get());

	// ビューポートの設定
	D3D12_VIEWPORT viewport = DxCreateLib::ViewLib::CreateViewport(FLOAT(backBufferWidth_), FLOAT(backBufferHeight_), 0, 0, 0.0f, 1.0f);
	D3D12_RECT scissorRect = DxCreateLib::ViewLib::CreateScissorRect(0, FLOAT(backBufferWidth_), 0, FLOAT(backBufferHeight_));
	DirectXCommand::sCommandList_->RSSetViewports(1, &viewport);
	DirectXCommand::sCommandList_->RSSetScissorRects(1, &scissorRect);
}

void DirectXCommon::RenderPostDraw()
{
	// Barrier
	D3D12_RESOURCE_BARRIER barrier = DxCreateLib::ResourceLib::GetResourceBarrier(rtvHandler_->GetRenderTexture(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

	DirectXCommand::sCommandList_->ResourceBarrier(1, &barrier);
}

void DirectXCommon::Finalize()
{
	// フェンスで終了の確認
	DirectXCommand::ExecuteCommandList(DirectXCommand::sCommandLoadList_.Get());
	DirectXCommand::WaitForFenceComplete();
	// コマンドリストのリセット
	DirectXCommand::ResetCloseCommandList(DirectXCommand::sCommandList_.Get());
	DirectXCommand::ResetCloseCommandList(DirectXCommand::sCommandLoadList_.Get(),DirectXCommand::sCommandLoadAllocator_.Get());
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
