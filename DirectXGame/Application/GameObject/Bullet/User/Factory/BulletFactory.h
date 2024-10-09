#pragma once
#include "../BulletsLists.h"
#include <string>

class InstancedUnit;

class BulletFactory
{
public:
	IBullet* CreateBullet(const std::string& name);
	//InstancedUnit* CreateBullet(const std::string& name);

};
