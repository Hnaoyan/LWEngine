#include "PostEffectRender.h"

void PostEffectRender::Initialize(ID3D12Device* device, ID3D12GraphicsCommandList* commandList)
{
	device_ = device;
	commandList_ = commandList;

}
