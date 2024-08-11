#include "ICamera.h"
#include "../../Base/DirectXCommon.h"
#include "../../Base/Utility/DxCreateLib.h"
#include "imgui.h"

#include <cassert>

void ICamera::Initialize()
{
	// スケールの設定
	transform_.scale = { 1.0f,1.0f,1.0f };
	// デバイス取得
	ID3D12Device* device = DirectXCommon::GetInstance()->GetDevice();
	data_.CreateConstantBuffer(device);
	// 行列の更新
	this->UpdateMatrix();
}

void ICamera::UpdateMatrix()
{
	// カメラ行列
	Matrix4x4 cameraMatrix = Matrix4x4::MakeAffineMatrix(transform_.scale, transform_.rotate, transform_.translate);
	// ビュー行列
	viewMatrix_ = Matrix4x4::MakeInverse(cameraMatrix);
	// プロジェクション行列
	projectionMatrix_ = Matrix4x4::MakePerspectiveFovMatrix(fov_, aspectRatio_, nearZ, farZ);
	// 角度
	fovAngle_ = fov_ * (float)(std::numbers::pi / 180.0f);
	// 前方ベクトル
	Matrix4x4 frontMat = Matrix4x4::MakeRotateYMatrix(transform_.rotate.y);
	frontVector_ = Matrix4x4::TransformVector3(kFrontVector, frontMat);
	frontVector_ = Vector3::Normalize(frontVector_);

	// 定数バッファに送信
	TransferMatrix();

}

void ICamera::TransferMatrix()
{
	// ビュー
	data_.cMap_->viewMatrix = viewMatrix_;
	// プロジェクション
	data_.cMap_->projectionMatrix = projectionMatrix_;
	Matrix4x4 translateMatrix = Matrix4x4::MakeTranslateMatrix(transform_.translate);
	// ワールド座標
	data_.cMap_->worldPosition = { translateMatrix.m[3][0],translateMatrix.m[3][1] ,translateMatrix.m[3][2] };

}

void ICamera::ImGuiDraw()
{
	
	ImGui::Begin("Camera");

	ImGui::DragFloat3("Position", &transform_.translate.x, 0.01f);
	ImGui::DragFloat3("Rotate", &transform_.rotate.x, 0.01f);

	ImGui::End();

}
