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
	// 外部データ用関数
	void GlobalValueInitialize();
	void GlobalValueUpdate();
private:
	// トランスフォーム
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// マテリアル
	std::unique_ptr<Material> material_;
	// 表示の切り替え
	bool isInvisible_ = false;
};

