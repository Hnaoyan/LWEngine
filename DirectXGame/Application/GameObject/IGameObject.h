#pragma once
#include "../../Engine/3D/Drawer/Model.h"

class IGameObject
{
protected:
	// 共通のシリアル
	static int32_t sSerialNumber;
	// 個体のシリアル
	int32_t commonSerialNumber_ = 0u;

public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model"></param>
	virtual void Initialize(Model* model);
	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update();
	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="desc"></param>
	virtual void Draw(ModelDrawDesc desc) = 0;
	/// <summary>
	/// ImGui描画
	/// </summary>
	/// <param name="name"></param>
	virtual void ImGuiDraw() = 0;

protected:
	// モデル
	Model* model_ = nullptr;
public:
	// ワールドトランスフォーム
	WorldTransform worldTransform_;

};
