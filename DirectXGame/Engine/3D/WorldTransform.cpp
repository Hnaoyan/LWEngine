#include "WorldTransform.h"
#include "../Base/DirectXCommon.h"
#include "../Base/Utility/DxCreateLib.h"

#include <cassert>

void WorldTransform::Initialize()
{
	// バッファーの作成
	this->CreateConstBuffer();
	// マッピング
	this->Map();
	// 行列の送信
	worldMatrix_ = Matrix4x4::MakeIdentity4x4();
	this->TransferMatrix();
}

void WorldTransform::UpdateMatrix()
{
	// ビルボードあり
	if (isBillBoard_) {

	}
	// ビルボードなし
	else {
		worldMatrix_ = Matrix4x4::MakeAffineMatrix(transform_.scale, transform_.rotate, transform_.translate);
	}

	// 親があれば
	if (parent_) {
		worldMatrix_ = Matrix4x4::Multiply(worldMatrix_, parent_->worldMatrix_);
	}

	// 定数バッファに送信
	TransferMatrix();
}

void WorldTransform::CreateConstBuffer()
{
	// デバイス取得
	ID3D12Device* device = DirectXCommon::GetInstance()->GetDevice();
	// リソース作成
	constBuffer_ = DxCreateLib::ResourceLib::CreateBufferResource(device, (sizeof(CBufferWorldTransform) + 0xff) & ~0xff);
}

void WorldTransform::TransferMatrix()
{
	constMap_->world = worldMatrix_;
}

void WorldTransform::Map()
{
	HRESULT result = S_FALSE;
	result = constBuffer_->Map(0, nullptr, (void**)&constMap_);
	assert(SUCCEEDED(result));
}
