#include "Animation.h"
#include "../System/DeltaTime.h"
#include "../3D/Loader.h"

void Animation::Initialize(ModelData* modelData)
{
	modelData_ = modelData;
	animData_ = modelData_->animData;
	skeleton_ = Loader::CreateSkeleton(modelData_->rootNode);
}

void Animation::UpdateAnimation()
{
	animationTime_ += kDeltaTime;
	animationTime_ = std::fmod(animationTime_, animData_.duration);
	// ノード取得
	//NodeAnimation& nodeAnim = animData_.nodeAnimations[modelData_->rootNode.name];

	//// トランスフォームの取得
	//transform_.translate = CalculateValue(nodeAnim.translate.keyframes, animationTime_);
	//transform_.rotate = CalculateValue(nodeAnim.rotate.keyframes, animationTime_);
	//transform_.scale = CalculateValue(nodeAnim.scale.keyframes, animationTime_);
	//
	//Quaternion normalizeRotate = Quaternion::Normalize(transform_.rotate);

	//// ローカル行列
	//localMatrix_ = Matrix4x4::MakeAffineMatrix(transform_.scale, normalizeRotate, transform_.translate);
	ApplyAnimation();
	UpdateSkelton();
}

void Animation::UpdateSkelton()
{
	// 全てのJointを更新。親が若いので通常ループで処理可能
	for (Joint& joint : skeleton_.joints) {
		joint.localMatrix = Matrix4x4::MakeAffineMatrix(joint.transform.scale, joint.transform.rotate, joint.transform.translate);

		if (joint.parent) {
			joint.skeletonSpaceMatrix = Matrix4x4::Multiply(joint.localMatrix, skeleton_.joints[*joint.parent].skeletonSpaceMatrix);
		}
		else {
			joint.skeletonSpaceMatrix = joint.localMatrix;
		}
		localMatrix_ = joint.skeletonSpaceMatrix;
	}

}

void Animation::ApplyAnimation()
{
	for (Joint& joint : skeleton_.joints) {

		if (auto it = animData_.nodeAnimations.find(joint.name); it != animData_.nodeAnimations.end()) {
			const NodeAnimation& rootNodeAnimation = (*it).second;
			joint.transform.translate = CalculateValue(rootNodeAnimation.translate.keyframes, animationTime_);
			joint.transform.rotate = CalculateValue(rootNodeAnimation.rotate.keyframes, animationTime_);
			joint.transform.scale = CalculateValue(rootNodeAnimation.scale.keyframes, animationTime_);
		}
	}


}
