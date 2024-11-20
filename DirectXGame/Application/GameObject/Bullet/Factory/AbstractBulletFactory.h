#pragma once
#include "../BulletEnums.h"
#include <string>

class IBullet;
class InstancedUnit;

class AbstractBulletFactory
{
public:
	virtual InstancedUnit* CreateUnit(const std::string& name) {
		name;
		return nullptr;
	}
	//template <typename T>
	//T* CreateBullet(const std::string& name);
};
