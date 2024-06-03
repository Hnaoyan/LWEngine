#pragma once
#include "../../Engine/3D/Drawer/Model.h"

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

};
