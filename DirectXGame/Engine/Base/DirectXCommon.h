#pragma once
#include "../Utility/Singleton.h"
#include "../WindowAPI/WindowAPI.h"
#include "../3D/Graphics/GraphicsPSO.h"
#include "DirectX/DirectXDevice.h"
#include "DirectX/DirectXCommand.h"
#include "DirectX/SwapChainManager.h"

#include "../3D/Descriptor/SRVHandler.h"
#include "../3D/Descriptor/RTVHandler.h"
#include "../3D/Descriptor/DSVHandler.h"


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

	ID3D12Device* device_ = nullptr;
	ID3D12GraphicsCommandList* commandList_ = nullptr;
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
	// srv
	std::unique_ptr<SRVHandler> srvHandler_;
	// rtv
	std::unique_ptr<RTVHandler> rtvHandler_;
	// dsv
	std::unique_ptr<DSVHandler> dsvHandler_;

public: // アクセッサ
	ID3D12Device* GetDevice() { return dxDevice_->GetDevice(); }
	ID3D12GraphicsCommandList* GetCommandList() { return DirectXCommand::commandList_.Get(); }

	SRVHandler* GetSrvHandler() { return srvHandler_.get(); }
	RTVHandler* GetRtvHandler() { return rtvHandler_.get(); }
	DSVHandler* GetDsvHandler() { return dsvHandler_.get(); }

private:
	// 記録時間(FPS固定用)
	std::chrono::steady_clock::time_point reference_;

};

