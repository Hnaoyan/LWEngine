#pragma once
#include "../../Engine/3D/Drawer/Model.h"
#include <string>

class PlSampleObject
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model"></param>
	void Initialize(Model* model);
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera"></param>
	void Draw(const ModelDrawDesc& desc);
	/// <summary>
	/// ImGui
	/// </summary>
	void ImGuiDraw();

private:
	// 
	Model* model_ = nullptr;
	//
	WorldTransform worldTransform_;
	// 
	Vector3 velocity_ = {};
};