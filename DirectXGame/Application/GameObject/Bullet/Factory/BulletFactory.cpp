#include "BulletFactory.h"
#include "../IBullet.h"

InstancedUnit* BulletFactory::CreateUnit(const std::string& name)
{
    InstancedUnit* unit = nullptr;
    if (name == "IBullet") {
        unit = new IBullet();
    }

    return unit;
}
