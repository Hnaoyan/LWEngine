#pragma once
#include <string>
#include <vector>
#include <map>
#include <optional>
#include "../3D/CBuffer.h"
#include "../3D/WorldTransform.h"

class ModelNode;

class Joint {
public:
	static int32_t Create(const ModelNode& node, const std::optional<int32_t>& parent, std::vector<Joint>& joints);

public:
	WorldTransform debug_;
	QuaternionTransform transform;	// Transform情報
	Matrix4x4 localMatrix;	// ローカル
	Matrix4x4 skeletonSpaceMatrix;	// SkeltonSpaceでの変換行列
	std::string name;	// 名前
	std::vector<int32_t> children;	// 子JointのIndexのリスト。いなければ空
	int32_t index;	// 自身のIndex
	std::optional<int32_t> parent;	// 親JointのIndex。いなければnull
};
