#pragma once
#include "AbstractBulletFactory.h"
#include "../BulletEnums.h"
#include <memory>

class BulletBuilder;
class IBullet;

class BulletFactory : public AbstractBulletFactory
{
public:
	InstancedUnit* CreateUnit(const std::string& name) override;

	std::unique_ptr<IBullet> CreateBullet(const BulletBuilder& builder, BulletType type);

};
