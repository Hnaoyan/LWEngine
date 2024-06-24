#pragma once
#include "../../Engine/3D/Drawer/Model.h"
#include "../../Engine/Animation/Animation.h"
#include "../../Engine/Animation/System/AnimationPlayer.h"
#include <string>

class AnimCubeObject
{
public:
	static int32_t sSerialNumber;
	//static std::string sObjectName;

	int32_t serialNumber_ = 0u;
	std::string objectName_;

public:
	void Initialize(Model* model);
	void Update();
	void Draw(const ModelDrawDesc& desc);

	void ImGuiDraw();

public:
	// アニメーション用のモデル
	Model* animModel_ = nullptr;

	// ワールドトランスフォーム
	WorldTransform worldTransform_;
	// アニメーション
	Animation animation_;

	AnimationData animData_;

	AnimationPlayer animPlayer_;

	float animationTime_ = 0;

	// アニメーション用のトランスフォーム
	QuaternionTransform transform_;
	// ローカル行列
	Matrix4x4 localMatrix_ = {};

};
