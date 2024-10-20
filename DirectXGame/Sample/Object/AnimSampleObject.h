#pragma once
#include "../../Engine/3D/Drawer/Model.h"
#include "../../Engine/Animation/Animation.h"
#include <string>

class Skydome;

class AnimSampleObject
{
public:
	static int32_t sSerialNumber;
	static std::string sObjectName;

	int32_t serialNumber_ = 0u;
	std::string objectName_;

	~AnimSampleObject() = default;

public:
	void Initialize(Model* model, Model* cube);
	void Update();
	void Draw(const ModelDrawDesc& desc);

	void ImGuiDraw();

public:
	// アニメーション用のモデル
	Model* animModel_ = nullptr;

	Model* debugCube_ = nullptr;

	uint32_t texture_ = 0u;

	std::unique_ptr<Skydome> debugSphere_;

	// ワールドトランスフォーム
	WorldTransform worldTransform_;
	// アニメーション
	Animation animation_;

};
