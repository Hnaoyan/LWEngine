#pragma once
#include "Engine/Math/Vector/Vector3.h"
#include "Engine/3D/Drawer/Triangle3D.h"
#include <vector>

class BulletTrail
{
public:
	// コンストラクタ
	BulletTrail();
	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="newPoint"></param>
	void UpdateTrail(const Vector3& newPoint);
	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera"></param>
	void Draw(ICamera* camera);

public:
	// 軌跡用の座標保存
	std::vector<Vector3> trailPoints_;
	// ポリゴン
	std::unique_ptr<Triangle3D> triangle_;
	// 長さ設定
	void SetLength(int32_t length) { maxLength = length; }

private:
	// 保存できる座標数
	int32_t maxLength = 0;

};
