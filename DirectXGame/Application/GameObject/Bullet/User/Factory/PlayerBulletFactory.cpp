#include "PlayerBulletFactory.h"
#include "../../IBullet.h"
//#include "../BulletsLists.h"

InstancedUnit* PlayerBulletFactory::CreateUnit(const std::string& name)
{
    InstancedUnit* unit = nullptr;
    if (name == "IBullet") {
        unit = new IBullet();
    }

    return unit;
}
