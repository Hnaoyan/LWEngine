#pragma once
#include "Engine/3D/ModelUtility/ModelRenderer.h"
#include <list>
#include <memory>

class Obstacle;
class CollisionManager;

class ObstacleManager
{
public:
	void Initialize();
	void Update();
	void CollisionUpdate(CollisionManager* manager);
	void Draw(ModelDrawDesc desc);
	void AddObstacle(const Vector2& position, const Vector2& size);
private:
	// 障害物リスト
	std::list<std::unique_ptr<Obstacle>> obstacles_;

};

