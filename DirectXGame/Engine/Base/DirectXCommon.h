#pragma once
#include "../Utility/Singleton.h"
#include "../WindowAPI/WindowAPI.h"
#include "../3D/Graphics/GraphicsPSO.h"
#include "DirectX/DirectXDevice.h"

class DirectXCommon : public Singleton<DirectXCommon>
{
public:
	void Initialize(WindowAPI* winApp, int32_t bufferWidth = WindowAPI::kWindowWidth, int32_t bufferHeight = WindowAPI::kWindowHeight);

	void PreDraw();
	void PostDraw();

private:
	// ウィンドウズポインタ
	WindowAPI* winApp_ = nullptr;
	// 画面のサイズ
	int32_t backBufferWidth_ = 0;
	int32_t backBufferHeight_ = 0;

	UINT fenceVal_ = 0;

	Microsoft::WRL::ComPtr<DirectXDevice> dxDevice_;

};

