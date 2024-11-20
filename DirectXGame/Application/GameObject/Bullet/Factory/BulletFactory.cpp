#include "BulletFactory.h"
#include "../IBullet.h"
#include "../BulletsPaths.h"
#include "Builder/BulletBuilder.h"

InstancedUnit* BulletFactory::CreateUnit(const std::string& name)
{
    InstancedUnit* unit = nullptr;
    if (name == "IBullet") {
        unit = new IBullet();
    }

    return unit;
}

std::unique_ptr<IBullet> BulletFactory::CreateBullet(const BulletBuilder& builder, BulletType type)
{
    std::unique_ptr<IBullet> bullet = builder.Build(type);

    return std::move(bullet);
}
