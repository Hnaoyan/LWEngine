#pragma once
#include <list>
#include <memory>

class Obstacle;

class ObstacleManager
{
public:
	void Initialize();
	void Update();
	void Draw();

private:
	// 障害物リスト
	std::list<std::unique_ptr<Obstacle>> obstacles_;

};

