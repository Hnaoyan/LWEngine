#include "InstancedUnit.h"

void InstancedUnit::Initialize()
{
	//transform_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };

	worldMatrix_ = Matrix4x4::MakeIdentity4x4();
	localMatrix_ = Matrix4x4::MakeIdentity4x4();

	isDead_ = false;
}

void InstancedUnit::Update()
{
	// ワールド計算
	worldMatrix_ = Matrix4x4::MakeAffineMatrix(transform_.scale, transform_.rotate, transform_.translate);

	// 親の処理
	if (parent_) {
		Matrix4x4 parent = parent_;
		worldMatrix_ = Matrix4x4::Multiply(worldMatrix_, parent);
	}
	// ローカル行列適応
	worldMatrix_ = Matrix4x4::Multiply(localMatrix_, worldMatrix_);
}
