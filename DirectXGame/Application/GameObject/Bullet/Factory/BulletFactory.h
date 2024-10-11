#pragma once
#include "AbstractBulletFactory.h"

class BulletFactory : public AbstractBulletFactory
{
public:
	InstancedUnit* CreateUnit(const std::string& name) override;

};
