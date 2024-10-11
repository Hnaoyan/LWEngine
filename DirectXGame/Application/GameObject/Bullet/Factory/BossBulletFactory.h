#pragma once
#include "AbstractBulletFactory.h"

class BossBulletFactory : public AbstractBulletFactory
{
public:
	InstancedUnit* CreateUnit(const std::string& name) override;

};