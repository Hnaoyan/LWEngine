#include "Animation.h"
#include "../LwLib/LwEngineLib.h"
#include "../3D/ModelLoader.h"
#include "../Base/DirectXCommon.h"
#include "imgui.h"

void Animation::Initialize(ModelData& modelData)
{
	modelData_ = modelData;
	animData_ = modelData_.animData;
	skeleton_ = Skeleton::Create(modelData_.rootNode);
	UpdateSkelton();
	skinCluster_ = SkinCluster::Create(DirectXCommon::GetInstance()->GetDevice(), skeleton_, modelData_);
	animationTime_ = 0;
}

void Animation::Update()
{
	//animationTime_ += nowFrame_ / 60.0f;
	//animationTime_ = std::fmod(animationTime_, animData_.duration);
	// 停止なら
	if (isStop_) {
		return;
	}
	// アニメーション再生処理
	switch (playType_)
	{
	case PlayBack::kForward:
		animationTime_ += 1.0f / 60.0f;
		animationTime_ = std::fmod(animationTime_, animData_.duration);
		break;
	case PlayBack::kReverse:
		animationTime_ -= 1.0f / 60.0f;

		if (animationTime_ < 0) {
			animationTime_ = animData_.duration;
		}
		break;
	}
	ApplyAnimation();

	UpdateSkelton();

	UpdateSkinCluster();
}

void Animation::UpdateSkelton()
{
	// 全てのJointを更新。親が若いので通常ループで処理可能
	for (Joint& joint : skeleton_.joints) {
		// ローカル行列作成
		joint.localMatrix = Matrix4x4::MakeAffineMatrix(joint.transform.scale, joint.transform.rotate, joint.transform.translate);
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
			joint.transform.translate = Animation::CalculateValue(rootNodeAnimation.translate.keyframes, animationTime_);
			joint.transform.rotate = Animation::CalculateValue(rootNodeAnimation.rotate.keyframes, animationTime_);
			//joint.transform.rotate = Quaternion::Normalize(joint.transform.rotate);
			joint.transform.scale = Animation::CalculateValue(rootNodeAnimation.scale.keyframes, animationTime_);
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

void Animation::ImGuiDraw(std::string parentName)
{
	//ImGui::Begin("anim");
	std::string name = parentName + "AnimFrame";

	ImGui::DragFloat(name.c_str(), &animationTime_, 0.01f);
	name = parentName + "Joints";
	if (ImGui::TreeNode(name.c_str())) {
		for (int i = 0; i < skeleton_.joints.size(); i++) {
			Vector3 localPos = skeleton_.joints[i].transform.translate;
			name = "LocalPos" + std::to_string(i);
			ImGui::DragFloat3(name.c_str(), &localPos.x,0.01f);
			skeleton_.joints[i].transform.translate = localPos;
		}
		ImGui::TreePop();
	}
	ImGui::Checkbox(parentName.c_str(), &isForward_);

	if (isForward_) {
		playType_ = PlayBack::kForward;
	}
	else {
		playType_ = PlayBack::kReverse;
	}
	name = parentName + "IsStop";
	ImGui::Checkbox(name.c_str(), &isStop_);

	//ImGui::End();
}
