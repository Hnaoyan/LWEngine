#include "BossBulletFactory.h"
#include "Application/GameObject/Boss/System/Bullet/BossBullet.h"

using namespace BossSystemContext;

InstancedUnit* BossBulletFactory::CreateUnit(const std::string& name)
{
    InstancedUnit* unit = nullptr;

    if (name == "Tracking") {
        unit = new TrackingBullet();
    }
    else if (name == "Normal") {
        unit = new NormalBullet();
    }

    return unit;
}
