#pragma once
#include "AbstractBulletFactory.h"

class PlayerBulletFactory : public AbstractBulletFactory
{
public:
	InstancedUnit* CreateUnit(const std::string& name) override;

};