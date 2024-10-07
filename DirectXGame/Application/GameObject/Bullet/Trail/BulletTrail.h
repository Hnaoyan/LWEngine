#pragma once
#include "Engine/Math/Vector/Vector3.h"
#include "Engine/3D/Drawer/Triangle3D.h"
#include <vector>

class BulletTrail
{
public:
	BulletTrail();

public:
	// 軌跡用の座標保存
	std::vector<Vector3> trailPoints_;
	// ポリゴン
	std::unique_ptr<Triangle3D> triangle_;
	
	void UpdateTrail(const Vector3& newPoint);
	void Draw(ICamera* camera);

	void SetLength(int32_t length) { maxLength = length; }

private:
	int32_t maxLength = 0;

};
