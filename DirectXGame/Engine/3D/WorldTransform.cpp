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
	// 単位行列
	worldMatrix_ = Matrix4x4::MakeIdentity4x4();
	localMatrix_ = Matrix4x4::MakeIdentity4x4();
	rotateQuaternion_ = {};
	// 行列の送信
	this->TransferMatrix();
	this->UpdateMatrix();
}

void WorldTransform::UpdateMatrix()
{
	// ビルボードあり
	if (isBillBoard_) {

	}
	// ビルボードなし
	else {
		if (isVectorRotate_) {
			// スケール
			Matrix4x4 scaleMat = Matrix4x4::MakeScaleMatrix(transform_.scale);
			// 回転
			Matrix4x4 rotateMat = Matrix4x4::DirectionToDirection({ 0.0f,0.0f,1.0f }, Vector3::Normalize(rotateDirect_));
			// 平行移動
			Matrix4x4 translateMat = Matrix4x4::MakeTranslateMatrix(transform_.translate);

			worldMatrix_ = Matrix4x4::Multiply(scaleMat, Matrix4x4::Multiply(rotateMat, translateMat));

		}
		else if (isQuaternion_) {

			axis_ = Vector3::Normalize(axis_);
			rotVector_ = Vector3::Normalize(rotVector_);

			Vector3 rotAxis = Vector3::Cross(axis_, rotVector_);
			rotAxis = Vector3::Normalize(rotAxis);
			float angle = std::acosf(Vector3::Dot(axis_, rotVector_));

			rotateQuaternion_ = Quaternion::MakeRotateAxisAngleQuaternion(rotAxis, angle);
			worldMatrix_ = Matrix4x4::MakeAffineMatrix(transform_.scale, rotateQuaternion_, transform_.translate);
		}
		else {
			worldMatrix_ = Matrix4x4::MakeAffineMatrix(transform_.scale, transform_.rotate, transform_.translate);
		}
	}

	// 親があれば
	if (parent_) {
		worldMatrix_ = Matrix4x4::Multiply(worldMatrix_, parent_->worldMatrix_);
	}
	else if (parentMatrix_) {
		worldMatrix_ = Matrix4x4::Multiply(worldMatrix_, parentMatrix_);
	}

	// ワールド行列をローカルに適応
	worldMatrix_ = Matrix4x4::Multiply(localMatrix_, worldMatrix_);
	

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
	constMap_->worldInverseTranspose = Matrix4x4::MakeTranspose(Matrix4x4::MakeInverse(worldMatrix_));
}

void WorldTransform::Map()
{
	HRESULT result = S_FALSE;
	result = constBuffer_->Map(0, nullptr, (void**)&constMap_);
	assert(SUCCEEDED(result));
}
