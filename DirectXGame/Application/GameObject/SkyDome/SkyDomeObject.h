#pragma once
#include "../IGameObject.h"
#include <memory>

/// <summary>
/// 天球クラス
/// </summary>
class SkyDomeObject
{
public:
	// 初期化
	void Initialize(Model* model);
	// 更新
	void Update();
	// ImGui
	void ImGuiDraw();
	// 描画
	void Draw(ModelDrawDesc desc);
	// 外部の
	void GlobalValueInitialize();
	void GlobalValueUpdate();
private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	std::unique_ptr<Material> material_;
};

