#pragma once
#include "../../Engine/3D/Drawer/Model.h"
#include "Engine/Collision/Collider/ColliderLists.h"
#include "../Collision/ColliderFilter.h"

class CollisionManager;

class IGameObject
{
public:
	ColliderObject SelectColliderObject(ColliderObject obj) {
		if (std::holds_alternative<Player*>(obj)) {
			return std::get<Player*>(obj);
		}
		else if (std::holds_alternative<IBullet*>(obj)) {
			return std::get<IBullet*>(obj);
		}

		return std::get<IGameObject*>(obj);
	}

public:
	// 共通のシリアル
	static int32_t sSerialNumber;
protected:
	// 個体のシリアル
	int32_t commonSerialNumber_ = 0u;
	// 死亡フラグ
	bool isDead_ = false;
	// デバッグ用の非表示フラグ
	bool isInvisible_ = false;

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
	/// <summary>
	/// コールバック関数
	/// </summary>
	/// <param name="target"></param>
	/// <param name="tag"></param>
	virtual void OnCollision(ColliderObject target) = 0;
	/// <summary>
	/// コライダーをマネージャに設定する関数
	/// </summary>
	/// <param name="collisionManager"></param>
	virtual void SetCollier(CollisionManager* collisionManager) = 0;
protected:
	/// <summary>
	/// グローバル変数の初期化
	/// </summary>
	virtual void InitializeGlobalValue() {};
	/// <summary>
	/// グローバル変数の更新
	/// </summary>
	virtual void UpdateGlobalValue() {};

public: // アクセッサ
	bool IsDead() const{ return isDead_; }
	void SetIsDead(bool isDead) { isDead_ = isDead; }

protected:
	// モデル
	Model* model_ = nullptr;

public:
	// ワールドトランスフォーム
	WorldTransform worldTransform_;
	// 前フレームの位置
	Vector3 prevPosition_ = {};

};
