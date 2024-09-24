#pragma once
#include "../ICollider.h"

class OBB : public ICollider
{
public: // 計算関数
	/// <summary>
	/// 分離軸に投影された軸成分から投影線分長の計算
	/// </summary>
	/// <param name="separate"></param>
	/// <param name="v1"></param>
	/// <param name="v2"></param>
	/// <param name="v3"></param>
	/// <returns></returns>
	static float LengthSeparateAxis(const Vector3& separate, const Vector3& v1, const Vector3& v2, const Vector3& v3 = {});
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
	void SetRadius(const Vector3& radius) { length_ = radius; }
public:
	// 衝突判定用
	// 座標
	Vector3 position_{};
	// 長さ
	Vector3 length_{};
	// 任意軸
	Vector3 arbitraryAxis[3]{};

};