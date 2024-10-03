#include "Skeleton.h"
#include "Engine/3D/ModelUtility/ModelData.h"

Skeleton Skeleton::Create(const ModelNode& rootNode)
{
    Skeleton instance;

	instance.root = Joint::Create(rootNode, {}, instance.joints);

	// 名前とindexのマッピングを行いアクセスしやすくする
	for (const Joint& joint : instance.joints) {
		instance.jointMap.emplace(joint.name, joint.index);
	}

	return Skeleton(instance);
}
