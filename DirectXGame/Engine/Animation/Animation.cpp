#include "Animation.h"
#include "../System/DeltaTime.h"
#include "../3D/Loader.h"
#include "../Base/DirectXCommon.h"
#include "imgui.h"

void Animation::Initialize(ModelData& modelData)
{
	modelData_ = modelData;
	animData_ = modelData_.animData;
	skeleton_ = Skeleton::Create(modelData_.rootNode);
	skinCluster_ = SkinCluster::Create(DirectXCommon::GetInstance()->GetDevice(), skeleton_, modelData_);
	animationTime_ = 0;
}

void Animation::UpdateAnimation()
{
	//animationTime_ = nowFrame_ / 180.0f;
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

	UpdateSkinCluster();
}

void Animation::UpdateSkelton()
{
	// 全てのJointを更新。親が若いので通常ループで処理可能
	for (Joint& joint : skeleton_.joints) {
		if (joint.parent) {
			joint.skeletonSpaceMatrix = Matrix4x4::Multiply(joint.localMatrix, skeleton_.joints[*joint.parent].skeletonSpaceMatrix);
		}
		else {
			joint.skeletonSpaceMatrix = joint.localMatrix;
		}
	}
}

void Animation::ApplyAnimation()
{
	for (Joint& joint : skeleton_.joints) {

		if (auto it = animData_.nodeAnimations.find(joint.name); it != animData_.nodeAnimations.end()) {
			const NodeAnimation& rootNodeAnimation = (*it).second;
			// トランスフォームの作成
			joint.transform.translate = CalculateValue(rootNodeAnimation.translate.keyframes, animationTime_);
			joint.transform.rotate = CalculateValue(rootNodeAnimation.rotate.keyframes, animationTime_);
			joint.transform.rotate = Quaternion::Normalize(joint.transform.rotate);
			joint.transform.scale = CalculateValue(rootNodeAnimation.scale.keyframes, animationTime_);
			// ローカル行列作成
			joint.localMatrix = Matrix4x4::MakeAffineMatrix(joint.transform.scale, joint.transform.rotate, joint.transform.translate);
		}
	}


}

void Animation::UpdateSkinCluster()
{
	for (size_t jointIndex = 0; jointIndex < skeleton_.joints.size(); ++jointIndex) {
		assert(jointIndex < skinCluster_.inverseBindPoseMatrices.size());
		skinCluster_.mappedPalette[jointIndex].skeletonSpaceMatrix =
			Matrix4x4::Multiply(skinCluster_.inverseBindPoseMatrices[jointIndex], skeleton_.joints[jointIndex].skeletonSpaceMatrix);
		skinCluster_.mappedPalette[jointIndex].skeletonSpaceInverseTransposeMatrix =
			Matrix4x4::MakeTranspose(Matrix4x4::MakeInverse(skinCluster_.mappedPalette[jointIndex].skeletonSpaceMatrix));
	}

}

void Animation::ImGuiDraw()
{
	//ImGui::Begin("anim");
	ImGui::DragFloat("AnimFrame",&animationTime_,0.01f);
	if (ImGui::TreeNode("Joints")) {
		for (int i = 0; i < skeleton_.joints.size(); i++) {
			Vector3 localPos = skeleton_.joints[i].transform.translate;
			std::string name = "LocalPos" + std::to_string(i);
			ImGui::DragFloat3(name.c_str(), &localPos.x,0.01f);
			skeleton_.joints[i].transform.translate = localPos;
		}
		ImGui::TreePop();
	}

	//ImGui::End();
}
