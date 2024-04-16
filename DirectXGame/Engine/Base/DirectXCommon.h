#pragma once
#include "../Utility/Singleton.h"
#include "../WindowAPI/WindowAPI.h"
#include "../3D/Graphics/GraphicsPSO.h"
#include "DirectX/DirectXDevice.h"
#include "DirectX/DirectXCommand.h"
#include "DirectX/SwapChainManager.h"

#include "../3D/Descriptor/SRVHandler.h"

#include <wrl.h>
#include <chrono>

class DirectXCommon : public Singleton<DirectXCommon>
{
public:
	void Initialize(WindowAPI* winApp, int32_t bufferWidth = WindowAPI::kWindowWidth, int32_t bufferHeight = WindowAPI::kWindowHeight);

	void PreDraw();
	void PostDraw();

private:

	void InitializeFixFPS();

	void UpdateFixFPS();

private:
	// ウィンドウズポインタ
	WindowAPI* winApp_ = nullptr;
	// 画面のサイズ
	int32_t backBufferWidth_ = 0;
	int32_t backBufferHeight_ = 0;

	UINT fenceVal_ = 0;

	// デバイス関係
	std::unique_ptr<DirectXDevice> dxDevice_;
	// コマンド関係
	std::unique_ptr<DirectXCommand> dxCommand_;
	// スワップチェーン・フェンス管理クラス
	std::unique_ptr<SwapChainManager> swapChainManager_;

private: // Heap
	std::unique_ptr<SRVHandler> srvHandler_;

private:
	// 記録時間(FPS固定用)
	std::chrono::steady_clock::time_point reference_;

};

