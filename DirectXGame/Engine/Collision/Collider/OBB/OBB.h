#pragma once
#include "../ICollider.h"

class OBB : public ICollider
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="shape"></param>
	void Initialize(const Vector3& radius, ColliderObject object);
	/// <summary>
	/// 更新
	/// </summary>
	void Update(const Vector3& worldPosition) override;

public:
	void SetRadius(const Vector3& radius) {
		radius_ = radius;
	}

public:
	// 衝突判定用
	Vector3 position_{};
	Vector3 length_{};
	Vector3 normalDirect[3]{};

private:
	// 半径サイズ
	Vector3 radius_ = {};

};