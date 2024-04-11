#include "DirectXCommon.h"

#include <cassert>

void DirectXCommon::Initialize(WindowAPI* winApp, int32_t bufferWidth, int32_t bufferHeight)
{
	assert(winApp);
	// 初期設定
	winApp_ = winApp;
	backBufferWidth_ = bufferWidth;
	backBufferHeight_ = bufferHeight;

	// 
	dxDevice_ = std::make_unique<DirectXDevice>();
	dxDevice_->Initialize();


}
