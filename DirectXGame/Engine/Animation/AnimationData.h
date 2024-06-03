#pragma once
#include <string>
#include <vector>
#include <map>
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
