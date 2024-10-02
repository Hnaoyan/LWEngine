#include "Joint.h"
#include <optional>
#include <vector>
#include "../3D/ModelUtility/ModelData.h"

int32_t Joint::Create(const ModelNode& node, const std::optional<int32_t>& parent, std::vector<Joint>& joints)
{
	Joint joint;
	
	// 確認用のトランスフォーム
	joint.debug_.Initialize();
	joint.debug_.transform_.scale = { 0.3f,0.3f,0.3f };

	// ジョイント作成
	joint.name = node.name;
	joint.localMatrix = node.localMatrix;
	joint.skeletonSpaceMatrix = Matrix4x4::MakeIdentity4x4();
	joint.transform = node.transform;
	joint.index = int32_t(joints.size());	// 現在登録されてる数をIndexに
	joint.parent = parent;

	joints.push_back(joint); // SkeletonのJoint列に追加

	for (const ModelNode& child : node.children) {
		// 子Jointを作成し、そのIndexを登録
		int32_t childIndex = Create(child, joint.index, joints);
		joints[joint.index].children.push_back(childIndex);
	}
	// 自身のIndexを返す
	return joint.index;
}
