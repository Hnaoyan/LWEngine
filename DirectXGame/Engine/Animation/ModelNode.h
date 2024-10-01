#pragma once
#include "../3D/ModelUtility/ModelData.h"

// ノード情報
class ModelNode {
public:
	// クォータニオン
	QuaternionTransform transform;
	// ローカル行列
	Matrix4x4 localMatrix;
	// 名前
	std::string name;
	// 子
	std::vector<ModelNode> children;
};
