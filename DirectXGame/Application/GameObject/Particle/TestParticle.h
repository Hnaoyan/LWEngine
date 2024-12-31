#pragma once
#include "Engine/3D/Drawer/Model.h"
#include "Engine/3D/ModelUtility/ModelRenderer.h"
#include "Engine/LwLib/LwEnginePaths.h"
#include <memory>

class TestParticle
{
public:
	void Initialie(Model* model);
	void Update();
	void Draw(ModelDrawDesc desc);
	void ActiveAception(const float& frame);

private:
	// モデル
	Model* model_ = nullptr;
	// マテリアル
	std::unique_ptr<Material> material_;
	// 
	WorldTransform worldTransform_;
	EulerTransform eulerTransform_;
	// 描画
	bool isInvisible_ = false;
	// アクションタイマー
	FrameTimer actionTimer_;
};
