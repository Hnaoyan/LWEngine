#include "ObstacleManager.h"
#include "../Obstacle.h"

void ObstacleManager::Initialize()
{
}

void ObstacleManager::Update()
{
	for (std::list<std::unique_ptr<Obstacle>>::iterator it = obstacles_.begin();
		it != obstacles_.end(); ++it)
	{
		(*it)->Update();
	}
}

void ObstacleManager::Draw(ModelDrawDesc desc)
{
	for (std::list<std::unique_ptr<Obstacle>>::iterator it = obstacles_.begin();
		it != obstacles_.end(); ++it)
	{
		(*it)->Draw(desc);
	}
}