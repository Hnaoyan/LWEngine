#pragma once
#include <string>
#include <vector>
#include <map>
#include <optional>
#include "../3D/CBuffer.h"

template <typename tValue>
struct Keyframe {
	float time;
	tValue value;
};

using KeyframeVector3 = Keyframe<Vector3>;
using KeyframeQuaternion = Keyframe<Quaternion>;

template<typename tValue>
struct AnimationCurve {
	std::vector<Keyframe<tValue>> keyframes;
};

struct NodeAnimation {
	AnimationCurve<Vector3> translate;
	AnimationCurve<Quaternion> rotate;
	AnimationCurve<Vector3> scale;
};

class AnimationData
{
public:
	// 全体の尺
	float duration;
	// NodeAnimationの集合、Node名で引けるように
	std::map<std::string, NodeAnimation> nodeAnimations;
	// ローカル行列
	Matrix4x4 localMatrix;
};

struct Joint {
	QuaternionTransform transform;	// Transform情報
	Matrix4x4 localMatrix;	// ローカル
	Matrix4x4 skeletonSpaceMatrix;	// SkeltonSpaceでの変換行列
	std::string name;	// 名前
	std::vector<int32_t> children;	// 子JointのIndexのリスト。いなければ空
	int32_t index;	// 自身のIndex
	std::optional<int32_t> parent;	// 親JointのIndex。いなければnull
};

struct Skeleton {
	int32_t root;	// RootJointのIndex
	std::map<std::string, int32_t> jointMap; // 名前とIndex
	std::vector<Joint> joints;	// ジョイント
};
