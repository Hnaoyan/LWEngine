#pragma once
#include <string>

class IBullet;
class InstancedUnit;

class AbstractBulletFactory
{
public:
	virtual IBullet* CreateBullet(const std::string& name) {
		name;
		return nullptr;
	}
	virtual InstancedUnit* CreateUnit(const std::string& name) {
		name;
		return nullptr;
	}
	//template <typename T>
	//T* CreateBullet(const std::string& name);
};
