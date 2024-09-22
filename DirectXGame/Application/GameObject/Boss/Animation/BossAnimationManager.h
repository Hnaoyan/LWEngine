#pragma once
#include "Engine/3D/Drawer/Model.h"
#include <vector>

class Boss;


class BossAnimationManager
{
public:
	// 階層パーツの情報
	struct Hierarchy
	{
		Model* model = nullptr;
		WorldTransform worldTransform;
	};
public:
	void Initialize(Boss* boss);
	void Update();
	void Draw(ICamera* camera, DrawDesc::LightDesc lightDesc);
private:
	void CreateHierarchy(std::string modelTag, const EulerTransform& transform);
private:
	// ボス本体
	Boss* boss_ = nullptr;
	// パーツごとの情報
	std::vector<Hierarchy> hierarchyObj_;

};
