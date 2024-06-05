#include "Animation.h"
#include "../System/DeltaTime.h"

void Animation::Initialize(ModelData* modelData)
{
	modelData_ = modelData;
	animData_ = modelData_->animData;

}

void Animation::UpdateAnimation()
{
	animationTime_ += kDeltaTime;
	animationTime_ = std::fmod(animationTime_, animData_.duration);
	// ノード取得
	NodeAnimation& nodeAnim = animData_.nodeAnimations[modelData_->rootNode.name];

	// トランスフォームの取得
	transform_.translate = CalculateValue(nodeAnim.translate.keyframes, animationTime_);
	transform_.rotate = CalculateValue(nodeAnim.rotate.keyframes, animationTime_);
	transform_.scale = CalculateValue(nodeAnim.scale.keyframes, animationTime_);
	
	Quaternion normalizeRotate = Quaternion::Normalize(transform_.rotate);

	// ローカル行列
	localMatrix_ = Matrix4x4::MakeAffineMatrix(transform_.scale, normalizeRotate, transform_.translate);

}
