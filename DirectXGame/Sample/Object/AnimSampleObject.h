#pragma once
#include "../../Engine/3D/Drawer/Model.h"
#include "../../Engine/Animation/Animation.h"

class AnimSampleObject
{
public:
	void Initialize(Model* model);
	void Update();
	void Draw(ICamera* camera);

	void ImGuiDraw();

public:
	// アニメーション用のモデル
	Model* animModel_ = nullptr;
	// ワールドトランスフォーム
	WorldTransform worldTransform_;
	// アニメーション
	Animation animation_;

};
