#pragma once
#include "../IGameObject.h"
#include "Engine/LwLib/Utillity/FrameTimer.h"
#include "Engine/Collision/Collider/ColliderLists.h"

class SampleEnemy : public IGameObject
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
	/// <summary>
	/// コールバック関数
	/// </summary>
	/// <param name="object"></param>
	void OnCollision(ColliderObject object);

public:
	Sphere* GetCollider() { return &collider_; }

private:
	Sphere collider_;

};