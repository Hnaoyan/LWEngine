#pragma once
#include "../IGameObject.h"
#include "System/PlayerSystemLists.h"

class Player : public IGameObject
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model"></param>
	void Initialize(Model* model) override;
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;
	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="desc"></param>
	void Draw(ModelDrawDesc desc) override;
	/// <summary>
	/// ImGui描画
	/// </summary>
	/// <param name="name"></param>
	void ImGuiDraw() override;

	WorldTransform* GetWorldTransform() { return &worldTransform_; }

private:
	/// <summary>
	/// システム関係の初期化
	/// </summary>
	void SystemInitialize();
	/// <summary>
	/// システム関係の更新
	/// </summary>
	void SystemUpdate();
	
public:
	Vector3 worldPosition_ = {};

private:
	OparationManager systemManager_;
};