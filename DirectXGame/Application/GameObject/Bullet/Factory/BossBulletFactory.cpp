#include "BossBulletFactory.h"
#include "Application/GameObject/Boss/System/Bullet/BossBullet.h"

InstancedUnit* BossBulletFactory::CreateUnit(const std::string& name)
{
    InstancedUnit* unit = nullptr;

    if (name == "TrackinBossSystemContextg") {
        unit = new BossSystemContext::TrackingBullet();
    }
    else if (name == "Normal") {
        unit = new BossSystemContext::NormalBullet();
    }

    return unit;
}
