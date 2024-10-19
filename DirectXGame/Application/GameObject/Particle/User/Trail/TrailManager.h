#pragma once
#include "Application/GameObject/Bullet/Trail/BulletTrail.h"
#include <vector>
#include <memory>

class ICamera;

class TrailManager
{
public:
	TrailManager();
	~TrailManager() = default;
public:
	void Draw(ICamera* camera);
	void AddTrail(std::unique_ptr<BulletTrail> trail);

private:
	// 軌跡インスタンスのコンテナ
	std::vector<std::unique_ptr<BulletTrail>> trails_;

};
