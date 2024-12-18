#pragma once
#include <string>
#include <vector>
#include <map>
#include <optional>

class ModelNode;
class Joint;

class Skeleton {
public:
	static Skeleton Create(const ModelNode& rootNode);


public:
	int32_t root = 0;	// RootJointのIndex
	std::map<std::string, int32_t> jointMap; // 名前とIndex
	std::vector<Joint> joints;	// ジョイント
};
