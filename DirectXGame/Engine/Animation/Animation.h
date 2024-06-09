#pragma once
#include "../3D/ModelData.h"
#include "AnimationData.h"
#include <cassert>
#include "../Math/MathLib.h"

class Animation
{
public:
	// モデルデータ
	ModelData* modelData_ = nullptr;
	// アニメーションデータ
	AnimationData animData_;
	// 現在の時間
	float animationTime_ = 0.0f;

	// アニメーション用のトランスフォーム
	QuaternionTransform transform_;
	// ローカル行列
	Matrix4x4 localMatrix_ = {};
	// SkeletonSpaceでの変換行列
	Matrix4x4  skeltonSpaceMatrix_; 

	Skeleton skeleton_;
	SkinCluster skinCluster_;
public:
	void Initialize(ModelData* modelData);
	void UpdateAnimation();

public:
	void UpdateSkelton();
	void ApplyAnimation();

	void UpdateSkinCluster();

public:
	inline static Vector3 CalculateValue(const std::vector<KeyframeVector3>& keyframes, float time) {
		assert(!keyframes.empty());
		if (keyframes.size() == 1 || time <= keyframes[0].time) {
			return keyframes[0].value;
		}

		for (size_t index = 0; index < keyframes.size() - 1; ++index) {
			size_t nextIndex = index + 1;
			// indexとnextIndexの2つのKeyframeを取得して範囲内に自国があるかを判定
			if (keyframes[index].time <= time && time <= keyframes[nextIndex].time) {
				// 範囲内を補間する
				float t = (time - keyframes[index].time / (keyframes[nextIndex].time - keyframes[index].time));
				return VectorLib::Lerp(keyframes[index].value, keyframes[nextIndex].value, t);
			}
		}
		return (*keyframes.rbegin()).value;
	}

	inline static Quaternion CalculateValue(const std::vector<KeyframeQuaternion>& keyframes, float time) {
		assert(!keyframes.empty());
		if (keyframes.size() == 1 || time <= keyframes[0].time) {
			return keyframes[0].value;
		}

		for (size_t index = 0; index < keyframes.size() - 1; ++index) {
			size_t nextIndex = index + 1;
			// indexとnextIndexの2つのKeyframeを取得して範囲内に自国があるかを判定
			if (keyframes[index].time <= time && time <= keyframes[nextIndex].time) {
				// 範囲内を補間する
				float t = (time - keyframes[index].time / (keyframes[nextIndex].time - keyframes[index].time));
				return Quaternion::Slerp(keyframes[index].value, keyframes[nextIndex].value, t);
			}
		}
		return (*keyframes.rbegin()).value;
	}

};
