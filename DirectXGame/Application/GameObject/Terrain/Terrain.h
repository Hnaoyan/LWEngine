#pragma once
#include "Engine/3D/Instancing/InstancedUnit.h"
#include "../../Collision/ColliderObject.h"
#include "Engine/Collision/Collider/ColliderLists.h"

class Terrain : public InstancedUnit
{
public:
	static uint32_t sSerialNumber;
	uint32_t serialNumber_ = 0u;

public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model"></param>
	void Initialize() override;
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;
	/// <summary>
	/// ImGui描画
	/// </summary>
	/// <param name="name"></param>
	void ImGuiDraw();
	/// <summary>
	/// コールバック関数
	/// </summary>
	/// <param name="object"></param>
	void OnCollision(ColliderObject object);
public: // アクセッサ
	AABB* GetCollider() { return &collider_; }
	EulerTransform GetTransform() { return transform_; }
private:
	AABB collider_;

};