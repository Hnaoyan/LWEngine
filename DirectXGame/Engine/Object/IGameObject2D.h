#pragma once
#include "Engine/Collision/ColliderShape.h"
#include "Engine/3D/Drawer/Model.h"
#include "Engine/Collision/Collider/ColliderLists.h"

#include <memory>

/// <summary>
/// 判定などを2Dで担う3Dオブジェクト用クラス
/// </summary>
class IGameObject2D
{
public:
	virtual void Initialize(Model* model);
	virtual void Update();
	virtual void Draw(ModelDrawDesc desc);
	virtual void ImGuiDraw() = 0;
	virtual void OnCollision([[maybe_unused]] ColliderObject target) {};

protected:
	

};
