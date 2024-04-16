#include "DirectXCommon.h"

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
	// DirectX関係の初期化
	dxDevice_ = std::make_unique<DirectXDevice>();
	dxDevice_->Initialize();

	dxCommand_ = std::make_unique<DirectXCommand>();
	dxCommand_->Initialize(dxDevice_->GetDevice());

	swapChainManager_ = std::make_unique<SwapChainManager>();
	swapChainManager_->Initialize(dxDevice_.get(), 1280, 720);

#pragma region Heap

	srvHandler_ = std::make_unique<SRVHandler>();
	srvHandler_->StaticInitialize(dxDevice_.get());


#pragma endregion


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
