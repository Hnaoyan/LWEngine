#pragma once
#include "../../Engine/3D/Drawer/Model.h"
#include "Engine/Collision/Collider/ColliderLists.h"

class IGameObject
{
public:
	ColliderObject SelectColliderObject(ColliderObject obj) {
		if (std::holds_alternative<Player*>(obj)) {
			return std::get<Player*>(obj);
		}
		else if (std::holds_alternative<SampleBullet*>(obj)) {
			return std::get<SampleBullet*>(obj);
		}

		return std::get<IGameObject*>(obj);
	}

protected:
	// 共通のシリアル
	static int32_t sSerialNumber;
	// 個体のシリアル
	int32_t commonSerialNumber_ = 0u;
	// それぞれのタグ
	std::string objectTag_;
public:
	// タグの取得
	std::string GetTag() { return objectTag_; }

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

public: // アクセッサ
	bool IsDead() { return isDead_; }

protected:
	// モデル
	Model* model_ = nullptr;
	// 死亡フラグ
	bool isDead_ = false;

public:
	// ワールドトランスフォーム
	WorldTransform worldTransform_;

};
