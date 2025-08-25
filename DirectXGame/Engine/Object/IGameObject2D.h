#pragma once
#include "Engine/Collision/ColliderShape.h"
#include "Engine/3D/Drawer/Model.h"
#include "Engine/3D/ModelUtility/ModelRenderer.h"
#include "Engine/Collision/Collider/ColliderLists.h"
#include <memory>

class CollisionManager;

/// <summary>
/// 判定などを2Dで担う3Dオブジェクト用クラス
/// </summary>
class IGameObject2D
{
public:	// コンストラクタ
	virtual ~IGameObject2D() = default;
public:
	virtual void Initialize(Model* model);
	virtual void Update();
	virtual void ColliderUpdate();
	virtual void Draw(ModelDrawDesc desc);
	virtual void ImGuiDraw() = 0;
	virtual void OnCollision([[maybe_unused]] ColliderObject target) {};
	virtual void SetCollision(CollisionManager* manager);

public: // アクセッサ
	bool IsDead() const { return isDead_; }
	WorldTransform* GetWorldTransform() { return &worldTransform_; }

protected:
	// モデル
	Model* model_ = nullptr;
	// ワールドトランスフォーム
	WorldTransform worldTransform_;
	// コライダー
	std::unique_ptr<Collider2DShape> collider_;
	std::unique_ptr<Material> material_;
	// 死亡フラグ
	bool isDead_ = false;

};
