#pragma once
#include "Engine/3D/WorldTransform.h"
#include "../ColliderShape.h"
#include "Application/Collision/ColliderObject.h"

class ICollider2D
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="shape"></param>
	virtual void Initialize(ColliderObject object);
	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update(const Vector2& worldPosition);

protected:

};
